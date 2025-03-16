#include "canvas.h"
#include "../application.h"
#include "../graph/graph.h"

#include <raylib.h>
#include <iostream>

void Canvas::resetCamera(){
    canvasCamera_.target = {0.0f, 0.0f};
    canvasCamera_.offset = {0.0f, 0.0f};
    canvasCamera_.rotation = 0.0f;
    canvasCamera_.zoom = 1.0f;
}

void Canvas::drawGrid() const{
    auto canvasPosition{canvasCamera_.target};
    int lineInterval{(canvasCamera_.zoom < .5f) ? 16 : 8};
    int screenHeight{static_cast<int>(GetScreenHeight() / canvasCamera_.zoom)};
    int screenWeight{static_cast<int>(GetScreenWidth() / canvasCamera_.zoom)};

    int leftMostVerticalLine{static_cast<int>(canvasPosition.x - static_cast<int>(canvasPosition.x) % lineInterval)};
    int verticalLineNumber{static_cast<int>(screenWeight / lineInterval)};

    for(auto i{0}; i < verticalLineNumber; i++){
        float x{static_cast<float>(leftMostVerticalLine + lineInterval * i)};

        DrawLineEx(
            {x, canvasCamera_.target.y},
            {x, canvasCamera_.target.y + screenHeight},
            (static_cast<int>(x) % (lineInterval * 3) == 0 ? 2.0f : 1.0f),
            {235, 235, 235, 255}
        );
    }

    int topMostHorizontalLine{static_cast<int>(canvasPosition.y - static_cast<int>(canvasPosition.y) % lineInterval)};
    int horizontalLineNumber{static_cast<int>(screenHeight / lineInterval)};

    for(auto i{0}; i < horizontalLineNumber; i++){
        float y{static_cast<float>(topMostHorizontalLine + lineInterval * i)};

        DrawLineEx(
            {canvasCamera_.target.x, y},
            {canvasCamera_.target.x + screenWeight, y},
            (static_cast<int>(y) % (lineInterval * 3) == 0 ? 2.0f : 1.0f),
            {235, 235, 235, 255}
        );
    }
}

void Canvas::drawMouse() const{
    switch(mode_){
    case Mode::VIEW: break;
    case Mode::SELECT: break;
    case Mode::MOVE: break;
    case Mode::PEN: drawPen(); break;
    case Mode::LINK: break; // drawLink(); break; // special case  
    case Mode::DRAG: break;
    case Mode::ERASER: drawEraser(); break;
    default: break;
    }
}

void Canvas::drawPen() const{

}

void Canvas::drawLink() const{
    if(linkFrom_){
        auto startingPosition{Application::instance().graph().getVertexPosition(linkFrom_.value())};
        auto thickness{Application::instance().graph().getEdgeThickness()};
        DrawLineEx(
            startingPosition,
            getMousePositionInCanvas(),
            thickness,
            GRAY
        );
        DrawCircleV(getMousePositionInCanvas(), thickness / 2, GRAY);
    }
}

void Canvas::drawEraser() const{
    
}

Vector2 Canvas::getMousePositionInCanvas() const{
    return GetScreenToWorld2D(GetMousePosition(), canvasCamera_);
}

void Canvas::updateHoveredItem(){
    auto currentHoveredVertex{Application::instance().graph().findVertex(
        getMousePositionInCanvas(), 
        Application::instance().graph().getVertexRadius() * 2.0f
    )};

    if(currentHoveredVertex && !hoveredVertexID_){
        hoveredVertexID_ = currentHoveredVertex;
        hoveredEdgeIDs_ = std::nullopt;
        std::cout << "   > Hovered Vertex: " 
                  << currentHoveredVertex.value()
                  << std::endl;
    }else if(!currentHoveredVertex){
        hoveredVertexID_ = std::nullopt;
    }

    if(currentHoveredVertex) return;

    auto currentHoveredEdge{Application::instance().graph().findEdge(
        getMousePositionInCanvas(), 
        Application::instance().graph().getEdgeThickness() * 1.25f
    )};

    if(currentHoveredEdge && !hoveredEdgeIDs_){
        hoveredEdgeIDs_ = currentHoveredEdge;
        std::cout << "   > Hovered Edge: " 
                  << currentHoveredEdge.value().first
                  << " -> " 
                  << currentHoveredEdge.value().second
                  << std::endl;
    }else if(!currentHoveredEdge){
        hoveredEdgeIDs_ = std::nullopt;
    }
}

void Canvas::switchMode(Mode mode){ 
    mode_ = mode;
    resetToolStatus();
}