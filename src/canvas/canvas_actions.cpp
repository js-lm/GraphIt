#include "canvas.hpp"
#include "system/application.hpp"
#include "graph/graph.hpp"
#include "actions_center/actions_center.hpp"
#include "actions_center/graph_action/general.hpp"
#include "actions_center/graph_action/bulk_removal.hpp"
#include "actions_center/graph_action/dye.hpp"
#include "actions_center/graph_action/reweigh.hpp"
#include "magic_enum.hpp"
#include "system/terminal_prefix.hpp"
#include "ui/ui.hpp"
#include "system/settings.hpp"

#include <optional>
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <iterator>

bool Canvas::isMouseOnCanvas() const{
    return Application::instance().ui().isMouseOnCanvas();
}

bool Canvas::isCanvasMouseButtonPressed(int key) const{
    if(!isMouseOnCanvas()) return false;
    return IsMouseButtonPressed(key);
}

void Canvas::updateMouseActions(){
    updateCamera();

    switch(mode_){
    case Mode::VIEW: break;
    case Mode::SELECT: updateSelect(); break;
    case Mode::PAN:    updateMove(); break;
    case Mode::PEN:    updatePen(); break;
    case Mode::LINK:   updateLink(); break;  
    case Mode::MOVE:   updateDrag(); break;
    case Mode::ERASER: updateEraser(); break;
    default: break;
    }
}

void Canvas::resetToolStatus(){
    linkFrom_ = std::nullopt;
    hoveredVertexID_ = std::nullopt;
    hoveredEdgeIDs_ = std::nullopt;
    selectedVertexIDs_.clear();
    selectedEdgeIDs_.clear();
}

void Canvas::updatePen(){
    if(isCanvasMouseButtonPressed(MOUSE_BUTTON_LEFT)
    && !Application::instance().graph().findVertex(getMousePositionInCanvas())
    ){
        Application::instance().actionCenter().addAction(
            std::make_unique<Action::AddVertex>(
                getMousePositionInCanvas(appSettings.gridIsSnapToGrid),
                appSettings.colorDebugPen
            )
        );
    }
}

void Canvas::updateLink(){
    if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) linkFrom_ = std::nullopt;
    if(isCanvasMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        if(!linkFrom_){
            linkFrom_ = hoveredVertexID_;

        }else if(hoveredVertexID_ && linkFrom_.value() != hoveredVertexID_.value()
             && !Application::instance().graph().areNeighbors(linkFrom_.value(), hoveredVertexID_.value())
        ){
            Application::instance().actionCenter().addAction(
                std::make_unique<Action::ConnectVertices>(
                    linkFrom_.value(), 
                    hoveredVertexID_.value(),
                    1.0f,
                    appSettings.colorDebugLink
                )
            );

            linkFrom_ = hoveredVertexID_;
        }
    }
}

void Canvas::updateEraser(){
    if(isCanvasMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        if(hoveredVertexID_){
            Application::instance().actionCenter().addAction(
                std::make_unique<Action::RemoveVertex>(
                    hoveredVertexID_.value()
                )
            );
        }else if(hoveredEdgeIDs_){
            Application::instance().actionCenter().addAction(
                std::make_unique<Action::DisconnectVertices>(
                    hoveredEdgeIDs_.value().first,
                    hoveredEdgeIDs_.value().second
                )
            );
        }
    }
}

void Canvas::updateMove(){
    updateScreenDragging(MOUSE_BUTTON_LEFT);
}

void Canvas::updateCamera(){
    updateScreenReset();
    updateScreenDragging();
    updateScreenZooming();
}

void Canvas::updateScreenDragging(int key){
    if(!isMouseOnCanvas()) return;
    // screen dragging
    if(IsMouseButtonPressed(key)){
        pivotPoint_ = getMousePositionInCanvas();
    }else if(IsMouseButtonDown(key)){
        Vector2 pivotDelta{Vector2Subtract(pivotPoint_, getMousePositionInCanvas())};
        canvasCamera_.target = Vector2Add(canvasCamera_.target, pivotDelta);
        pivotPoint_ = getMousePositionInCanvas();
    }
}

void Canvas::updateScreenReset(){
    if(!isMouseOnCanvas()) return;
    // double middle clicks to reset camera
    if(doubleMiddleClickCD_){
        if(IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)
        && Vector2Distance(doubleMiddleClickStartPosition_, GetMousePosition()) < 10.0f
        ){
            resetCamera();
            setZoomBarTimer();
            doubleMiddleClickCD_ = std::nullopt;
        }else{
            *doubleMiddleClickCD_ -= 1.0f * GetFrameTime();

            if(doubleMiddleClickCD_.value() <= 0) doubleMiddleClickCD_ = std::nullopt;
        }
    }else if(IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)){
        doubleMiddleClickCD_ = .5f;
        doubleMiddleClickStartPosition_ = GetMousePosition();
    }
}

void Canvas::updateScreenZooming(){
    if(!isMouseOnCanvas()) return;
    // zooming
    auto zoom{GetMouseWheelMove()};
    if(zoom){
        Vector2 pivot{getMousePositionInCanvas()};
        float zoomSpeed{.1f * zoomFactorTemp_};
        zoomFactorTemp_ += zoomSpeed * GetMouseWheelMove();
        zoomFactorTemp_ = Clamp(zoomFactorTemp_, .2f, 2.0f);
        canvasCamera_.zoom = zoomFactorTemp_;

        if(abs(1.0f - canvasCamera_.zoom) < .075f){
            canvasCamera_.zoom = 1.0f;
        }
        
        canvasCamera_.target = Vector2Add(
            canvasCamera_.target, 
            Vector2Subtract(pivot, getMousePositionInCanvas())
        );

        setZoomBarTimer();

    }else if(lostScrollCD_){
        *lostScrollCD_ -= GetFrameTime();

        if(lostScrollCD_.value() <= 0.0f){
            lostScrollCD_ = std::nullopt;
        }
    }
}

void Canvas::updateDrag(){
    if(vertexToDrag_){
        bool snapToGrid{appSettings.gridIsSnapToGrid};

        Application::instance().graph().updateVertexPosition(vertexToDrag_.value(), getMousePositionInCanvas(snapToGrid));

        if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
            Application::instance().actionCenter().addAction(
                std::make_unique<Action::MoveVertex>(
                    vertexToDrag_.value(),
                    vertexOriginalPosition_,
                    getMousePositionInCanvas(snapToGrid)
                )
            );

            vertexToDrag_ = std::nullopt;
            vertexOriginalPosition_ = {0, 0};
        }
    }

    if(isCanvasMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        if(!vertexToDrag_ && hoveredVertexID_){
            vertexToDrag_ = hoveredVertexID_;
            vertexOriginalPosition_ = Application::instance().graph().getVertexPosition(vertexToDrag_.value());
        }
    }
}

void Canvas::updateSelect(){
    if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && startFrom_){
        if(handleIndividualSelection()) return;
        handleGroupSelection();
    }

    if(isCanvasMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        handleSelectionStart();
    }

    if(!selectedEdgeIDs_.empty()){
        handleEdgeWeightInput();
    }
}

void Canvas::handleSelectionStart(){
    startFrom_ = getMousePositionInCanvas();
    selectedVertex_ = hoveredVertexID_;
    selectedEdge_ = hoveredEdgeIDs_;

    if(!(IsKeyDown(KEY_LEFT_SHIFT) 
     || IsKeyDown(KEY_LEFT_CONTROL) 
     || IsKeyDown(KEY_LEFT_SUPER)
    )){
        reweighSelectedEdges();
        selectedVertexIDs_.clear();
        selectedEdgeIDs_.clear();
        weightInputString_.clear();
    }
}

bool Canvas::handleIndividualSelection(){
    if(selectedVertex_){
        if(hoveredVertexID_ && selectedVertex_.value() == hoveredVertexID_.value()){
            if(selectedVertexIDs_.find(selectedVertex_.value()) != selectedVertexIDs_.end()){
                selectedVertexIDs_.erase(selectedVertex_.value());
            }else{
                selectedVertexIDs_.emplace(selectedVertex_.value());
            }
            startFrom_ = std::nullopt;
            return true;
        }
    }else if(selectedEdge_){
        if(hoveredEdgeIDs_ && selectedEdge_.value() == hoveredEdgeIDs_.value()){
            if(selectedEdgeIDs_.find(selectedEdge_.value()) != selectedEdgeIDs_.end()){
                selectedEdgeIDs_.erase(selectedEdge_.value());
            }else{
                selectedEdgeIDs_.emplace(selectedEdge_.value());
            }
            startFrom_ = std::nullopt;
            return true;
        }
    }

    return false;
}

void Canvas::handleGroupSelection(){
    auto rectangle{normalizeRectangle(startFrom_.value(), getMousePositionInCanvas())};

    auto verticesVector{Application::instance().graph().findVertex(rectangle)};
    auto edgesVector{Application::instance().graph().findEdge(rectangle)};

    selectedVertexIDs_.insert(verticesVector.begin(), verticesVector.end());
    selectedEdgeIDs_.insert(edgesVector.begin(), edgesVector.end());

    startFrom_ = std::nullopt;

    printUpdatePrefix();
    std::cout << "Selected: " << std::endl;
    std::cout << "\t> Vertices ";
    for(const auto &vertex : selectedVertexIDs_){
        std::cout << vertex << " ";
    }
    std::cout << std::endl;
    std::cout << "\t> Edges ";
    for(const auto &edge : selectedEdgeIDs_){
        std::cout << "(" << edge.first << "," << edge.second << ") ";
    }
    std::cout << std::endl;
}

void Canvas::handleEdgeWeightInput(){
    char key{static_cast<char>(GetCharPressed())};

    if(((key >= '0' && key <= '9') || key == '.')
    && weightInputString_.size() < 7
    ){
        if(key != '.' 
        || weightInputString_.find(".") == std::string::npos
        ){
            weightInputString_ += key;
        }
    }else if(IsKeyPressed(KEY_BACKSPACE) && !weightInputString_.empty()){
        weightInputString_.erase(weightInputString_.end() - 1, weightInputString_.end());
    }else if(IsKeyPressed(KEY_ENTER)){
        reweighSelectedEdges();
    }
}

void Canvas::reweighSelectedEdges(){
    if(weightInputString_.empty()
    || selectedEdgeIDs_.empty()
    ){
        return;
    }
    float newWeight{std::stof(weightInputString_)};
    Application::instance().actionCenter().addAction(
        std::make_unique<Action::Reweigh>(
            std::vector(selectedEdgeIDs_.begin(), selectedEdgeIDs_.end()),
            newWeight
        )
    );
    selectedVertexIDs_.clear();
    selectedEdgeIDs_.clear();
    weightInputString_.clear();
}

void Canvas::doBulkDeleteVertices(){
    if(mode_ != Mode::SELECT || selectedVertexIDs_.empty()) return;

    Application::instance().actionCenter().addAction(
        std::make_unique<Action::BulkRemoveVertices>(
            std::vector<VertexID>(selectedVertexIDs_.begin(), selectedVertexIDs_.end())
        )
    );

    resetToolStatus();
}

void Canvas::doBulkDeleteEdges(){
    if(mode_ != Mode::SELECT || selectedEdgeIDs_.empty()) return;

    Application::instance().actionCenter().addAction(
        std::make_unique<Action::BulkRemoveEdges>(
            std::vector<EdgeID>(selectedEdgeIDs_.begin(), selectedEdgeIDs_.end())
        )
    );

    resetToolStatus();
}

void Canvas::doBulkDelete(){
    if(mode_ != Mode::SELECT || (selectedEdgeIDs_.empty() && selectedVertexIDs_.empty())) return;

    Application::instance().actionCenter().addAction(
        std::make_unique<Action::BulkRemove>(
            std::vector<VertexID>(selectedVertexIDs_.begin(), selectedVertexIDs_.end()),
            std::vector<EdgeID>(selectedEdgeIDs_.begin(), selectedEdgeIDs_.end())
        )
    );

    resetToolStatus();
}

void Canvas::doDyeVertex(){
    if(selectedVertexIDs_.empty()) return;
    Application::instance().actionCenter().addAction(
        std::make_unique<Action::DyeVertex>(
            std::vector<VertexID>(selectedVertexIDs_.begin(), selectedVertexIDs_.end()),
            appSettings.colorDebugDye
        )
    );
}

void Canvas::doDyeEdge(){
    if(selectedEdgeIDs_.empty()) return;
    Application::instance().actionCenter().addAction(
        std::make_unique<Action::DyeEdge>(
            std::vector<EdgeID>(selectedEdgeIDs_.begin(), selectedEdgeIDs_.end()),
            appSettings.colorDebugDye
        )
    );
}

void Canvas::doDye(){
    if(selectedVertexIDs_.empty() && selectedEdgeIDs_.empty()) return;
    Application::instance().actionCenter().addAction(
        std::make_unique<Action::Dye>(
            std::vector<VertexID>(selectedVertexIDs_.begin(), selectedVertexIDs_.end()),
            std::vector<EdgeID>(selectedEdgeIDs_.begin(), selectedEdgeIDs_.end()),
            appSettings.colorDebugDye
        )
    );
}