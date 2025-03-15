#include "canvas.h"
#include "../application.h"
#include "../graph/graph.h"

#include <raylib.h>

void Canvas::resetCamera(){
    canvasCamera_.target = {0.0f, 0.0f};
    canvasCamera_.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
    canvasCamera_.rotation = 0.0f;
    canvasCamera_.zoom = 1.0f;
}

void Canvas::drawGrid() const{

}

void Canvas::drawPen() const{

}

void Canvas::drawLink() const{

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