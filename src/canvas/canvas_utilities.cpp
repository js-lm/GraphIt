#include "canvas.h"
#include "../application.h"
#include "../graph/graph.h"

#include <raylib.h>

void Canvas::resetCamera(){
    canvasCamera_.target = {0.0f, 0.0f};
    canvasCamera_.offset = {0.0f, 0.0f};
    canvasCamera_.rotation = 0.0f;
    canvasCamera_.zoom = 1.0f;
}

void Canvas::drawGrid() const{
    auto canvasPosition{canvasCamera_.target};
    int lineInterval{8};

    int verticalLineInterval{lineInterval};
    int leftMostVerticalLine{static_cast<int>(canvasPosition.x - static_cast<int>(canvasPosition.x) % verticalLineInterval)};
    int verticalLineNumber{static_cast<int>(GetScreenWidth() / verticalLineInterval)};

    for(auto i{0}; i < verticalLineNumber; i++){
        float x{static_cast<float>(leftMostVerticalLine + verticalLineInterval * i)};

        DrawLineEx(
            {x, canvasCamera_.target.y},
            {x, canvasCamera_.target.y + GetScreenHeight()},
            (static_cast<int>(x) % 24 == 0 ? 2.0f : 1.0f),
            {235, 235, 235, 255}
        );
    }

    int horizontalLineInterval{lineInterval};
    int topMostHorizontalLine{static_cast<int>(canvasPosition.y - static_cast<int>(canvasPosition.y) % horizontalLineInterval)};
    int horizontalLineNumber{static_cast<int>(GetScreenHeight() / horizontalLineInterval)};

    for(auto i{0}; i < horizontalLineNumber; i++){
        float y{static_cast<float>(topMostHorizontalLine + horizontalLineInterval * i)};

        DrawLineEx(
            {canvasCamera_.target.x, y},
            {canvasCamera_.target.x + GetScreenWidth(), y},
            (static_cast<int>(y) % 24 == 0 ? 2.0f : 1.0f),
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
    case Mode::LINK: drawLink(); break;  
    case Mode::DRAG: break;
    case Mode::ERASER: drawEraser(); break;
    default: break;
    }
}

void Canvas::drawPen() const{

}

void Canvas::drawLink() const{

}

void Canvas::drawEraser() const{
    
}

Vector2 Canvas::getMousePositionInCanvas() const{
    return GetScreenToWorld2D(GetMousePosition(), canvasCamera_);
}

void Canvas::updateHoveredItem(){
    hoveredVertexID_ = Application::instance().graph().findVertex(
        getMousePositionInCanvas(), 
        Application::instance().graph().getVertexRadius()
    );

    if(hoveredVertexID_){
        hoveredEdgeIDs_ = std::nullopt;
        return;
    }

    hoveredEdgeIDs_ = Application::instance().graph().findEdge(
        getMousePositionInCanvas(), 
        Application::instance().graph().getEdgeThickness()
    );
}

void Canvas::switchMode(Mode mode){ 
    mode_ = mode;
    resetToolStatus();
}