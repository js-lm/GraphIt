#include "canvas.h"
#include "../application.h"
#include "../graph/graph.h"
#include "../actions_center/actions_center.h"
#include "../actions_center/graph_action/graph_action.h"
#include "lib/magic_enum.hpp"
#include "../configs/terminal_prefix.h"

#include <optional>
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <iterator>

bool Canvas::isCanvasMouseButtonPressed(int key){
    if(!CheckCollisionPointRec(GetMousePosition(), {0, 40, 1000, 640})) return false;
    return IsMouseButtonPressed(key);
}

void Canvas::updateMouseActions(){
    // if(mode_ != Mode::SELECT) updateCamera();
    updateCamera();

    switch(mode_){
    case Mode::VIEW: break;
    case Mode::SELECT: updateSelect(); break;
    case Mode::MOVE:   updateMove(); break;
    case Mode::PEN:    updatePen(); break;
    case Mode::LINK:   updateLink(); break;  
    case Mode::DRAG:   updateDrag(); break;
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
    if(isCanvasMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        Application::instance().actionCenter().addAction(
            std::make_unique<Action::AddVertex>(
                getMousePositionInCanvas(isSnapToGridEnabled_),
                BLACK // TODO: color
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
                    hoveredVertexID_.value()
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
    // double middle clicks to reset camera
    if(doubleMiddleClickCD_){
        if(IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)){
            resetCamera();
            doubleMiddleClickCD_ = std::nullopt;
        }else{
            *doubleMiddleClickCD_ -= 1.0f * GetFrameTime();

            if(doubleMiddleClickCD_.value() <= 0) doubleMiddleClickCD_ = std::nullopt;
        }
    }else if(IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)){
        doubleMiddleClickCD_ = .2f;
    }
}

void Canvas::updateScreenZooming(){
    // zooming
    auto zoom{GetMouseWheelMove()};
    if(zoom){
        Vector2 pivot{getMousePositionInCanvas()};
        canvasCamera_.zoom += .2f * GetMouseWheelMove();

        canvasCamera_.zoom = Clamp(canvasCamera_.zoom, .2f, 2.0f);

        canvasCamera_.target = Vector2Add(
            canvasCamera_.target, 
            Vector2Subtract(pivot, getMousePositionInCanvas())
        );
    }
}

void Canvas::updateDrag(){
    if(vertexToDrag_){
        Application::instance().graph().updateVertexPosition(vertexToDrag_.value(), getMousePositionInCanvas(isSnapToGridEnabled_));

        if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
            Application::instance().actionCenter().addAction(
                std::make_unique<Action::MoveVertex>(
                    vertexToDrag_.value(),
                    vertexOriginalPosition_,
                    getMousePositionInCanvas(isSnapToGridEnabled_)
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
        // single selection
        if(selectedVertex_){
            auto current{Application::instance().graph().findVertex(
                getMousePositionInCanvas(), 
                Application::instance().graph().getVertexRadius()
            )};

            if(selectedVertex_.value() == current.value_or(-1)){
                if(selectedVertexIDs_.find(selectedVertex_.value()) != selectedVertexIDs_.end()){
                    selectedVertexIDs_.erase(selectedVertex_.value());
                }else{
                    selectedVertexIDs_.emplace(selectedVertex_.value());
                }
                startFrom_ = std::nullopt;
                return;
            }
        }else if(selectedEdge_){
            auto current{Application::instance().graph().findEdge(
                getMousePositionInCanvas(), 
                Application::instance().graph().getEdgeThickness() * 2.0f
            )};

            if(selectedEdge_.value() == current.value_or(std::pair{-1, -1})){
                if(selectedEdgeIDs_.find(selectedEdge_.value()) != selectedEdgeIDs_.end()){
                    selectedEdgeIDs_.erase(selectedEdge_.value());
                }else{
                    selectedEdgeIDs_.emplace(selectedEdge_.value());
                }
                startFrom_ = std::nullopt;
                return;
            }
        }

        // group selection
        auto rectangle{normalizeRectangle(startFrom_.value(), getMousePositionInCanvas())};

        auto verticesVector{Application::instance().graph().findVertex({
            rectangle
        })};

        auto edgesVector{Application::instance().graph().findEdge({
            rectangle
        })};

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

    if(isCanvasMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        startFrom_ = getMousePositionInCanvas();
        selectedVertex_ = Application::instance().graph().findVertex(
            getMousePositionInCanvas(), 
            Application::instance().graph().getVertexRadius() * 2.0f
        );

        selectedEdge_ = Application::instance().graph().findEdge(
            getMousePositionInCanvas(), 
            Application::instance().graph().getEdgeThickness() * 2.0f
        );

        if(!(IsKeyDown(KEY_LEFT_SHIFT) 
        || IsKeyDown(KEY_LEFT_CONTROL) 
        || IsKeyDown(KEY_LEFT_SUPER))
        ){
            selectedVertexIDs_.clear();
            selectedEdgeIDs_.clear();
        }
    }
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