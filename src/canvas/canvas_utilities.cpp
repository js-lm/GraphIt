#include "canvas.h"
#include "system/application.h"
#include "graph/graph.h"
#include "lib/magic_enum.hpp"
#include "system/terminal_prefix.h"

#include <raylib.h>
#include <iostream>
#include <cmath>
#include <raymath.h>

void Canvas::resetCamera(){
    canvasCamera_.target = {0.0f, 0.0f};
    canvasCamera_.offset = {0.0f, 0.0f};
    canvasCamera_.rotation = 0.0f;
    canvasCamera_.zoom = 1.0f;
}

void Canvas::drawGrid() const{
    if(!Application::getValue<Setting, bool>(Setting::GRID_SHOW)) return;

    auto canvasPosition{canvasCamera_.target};
    int cellSize{Application::getValue<Setting, int>(Setting::GRID_CELL_SIZE)};
    int lineInterval{Application::getValue<Setting, int>(Setting::GRID_SUBDIVISION_SIZE)};
    int screenHeight{static_cast<int>(GetScreenHeight() / canvasCamera_.zoom)};
    int screenWeight{static_cast<int>(GetScreenWidth() / canvasCamera_.zoom)};

    int leftMostVerticalLine{static_cast<int>(canvasPosition.x - static_cast<int>(canvasPosition.x) % cellSize)};
    int verticalLineNumber{static_cast<int>(screenWeight / cellSize)};

    for(auto i{0}; i < verticalLineNumber; i++){
        float x{static_cast<float>(leftMostVerticalLine + cellSize * i)};

        DrawLineEx(
            {x, canvasCamera_.target.y},
            {x, canvasCamera_.target.y + screenHeight},
            (static_cast<int>(x) % (cellSize * lineInterval) == 0 ? 2.0f : 1.0f),
            {235, 235, 235, 255}
        );
    }

    int topMostHorizontalLine{static_cast<int>(canvasPosition.y - static_cast<int>(canvasPosition.y) % cellSize)};
    int horizontalLineNumber{static_cast<int>(screenHeight / cellSize)};

    for(auto i{0}; i < horizontalLineNumber; i++){
        float y{static_cast<float>(topMostHorizontalLine + cellSize * i)};

        DrawLineEx(
            {canvasCamera_.target.x, y},
            {canvasCamera_.target.x + screenWeight, y},
            (static_cast<int>(y) % (cellSize * lineInterval) == 0 ? 2.0f : 1.0f),
            {235, 235, 235, 255}
        );
    }
}

void Canvas::drawMouse() const{
    switch(mode_){
    case Mode::VIEW: break;
    case Mode::SELECT: drawSelect(); break; // drawSelect(); // below UI
    case Mode::PAN: break;
    case Mode::PEN: drawPen(); break;
    case Mode::LINK: break; // drawLink(); break; // below UI and Vertices
    case Mode::MOVE: break;
    case Mode::ERASER: drawEraser(); break;
    default: break;
    }
}

void Canvas::drawPen() const{
    bool snapToGrid{Application::getValue<Setting, bool>(Setting::GRID_IS_SNAP_TO_GRID)};
    DrawCircleV(
        getMousePositionInCanvas(snapToGrid), 
        Application::instance().getValue<Setting, float>(Setting::GRAPH_VERTEX_RADIUS), 
        penColor_
    );
}

void Canvas::drawLink() const{
    if(linkFrom_){
        auto startingPosition{Application::instance().graph().getVertexPosition(linkFrom_.value())};
        auto thickness{Application::instance().getValue<Setting, float>(Setting::GRAPH_EDGE_THICKNESS)};
        DrawLineEx(
            startingPosition,
            getMousePositionInCanvas(),
            thickness,
            linkColor_
        );
        DrawCircleV(getMousePositionInCanvas(), thickness / 2, linkColor_);
    }
}

void Canvas::drawEraser() const{
    
}

void Canvas::drawSelect() const{
    if(!startFrom_) return;

    Rectangle rectangle{normalizeRectangle(startFrom_.value(), getMousePositionInCanvas())};

    DrawRectangleRec(rectangle, Fade(BLUE, .5f));
    DrawRectangleLinesEx(rectangle, 2.0f, BLUE);
}

Vector2 Canvas::getMousePositionInCanvas(bool snap) const{
    auto position{GetScreenToWorld2D(GetMousePosition(), canvasCamera_)};
    return snap ? snapVector(position) : position;
}

Vector2 Canvas::snapVector(Vector2 vector) const{
    int cellSize{Application::instance().getValue<Setting, int>(Setting::GRID_CELL_SIZE)};
    vector.x = std::round(vector.x / cellSize) * cellSize;
    vector.y = std::round(vector.y / cellSize) * cellSize;
    return vector;
}

void Canvas::updateHoveredItem(){
    auto currentHoveredVertex{Application::instance().graph().findVertex(
        getMousePositionInCanvas(), 
        Application::instance().getValue<Setting, float>(Setting::GRAPH_VERTEX_RADIUS) * 1.5f
    )};

    if(currentHoveredVertex && !hoveredVertexID_){
        hoveredEdgeIDs_ = std::nullopt;
        printUpdatePrefix();
        std::cout << "Hovered Vertex: " 
                  << currentHoveredVertex.value()
                  << TERMINAL_RESET << std::endl;
    }

    hoveredVertexID_ = currentHoveredVertex;
    if(hoveredVertexID_) return;

    auto currentHoveredEdge{Application::instance().graph().findEdge(
        getMousePositionInCanvas(), 
        Application::instance().getValue<Setting, float>(Setting::GRAPH_EDGE_THICKNESS) * 1.25f
    )};

    if(currentHoveredEdge && !hoveredEdgeIDs_){
        printUpdatePrefix();
        std::cout << "Hovered Edge: " 
                  << currentHoveredEdge.value().first
                  << " -> " 
                  << currentHoveredEdge.value().second
                  << std::endl;
    }

    hoveredEdgeIDs_ = currentHoveredEdge;
}

void Canvas::switchMode(Mode mode){ 
    mode_ = mode;
    printCanvasPrefix();
    std::cout << "Switching mode to " 
              << magic_enum::enum_name(mode) 
              << std::endl;
    resetToolStatus();
}

Rectangle Canvas::normalizeRectangle(Vector2 startingPosition, Vector2 endingPosition) const{
    float left{std::min(startingPosition.x, endingPosition.x)};
    float top{std::min(startingPosition.y, endingPosition.y)};
    float width{std::abs(startingPosition.x - endingPosition.x)};
    float height{std::abs(startingPosition.y - endingPosition.y)};

    return {left, top, width, height};
}