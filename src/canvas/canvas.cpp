#include "canvas.h"
#include "../application.h"
#include "../graph/graph.h"

#include <raylib.h>

Canvas::Canvas()
    : mode_(Mode::VIEW)
    , pivotPoint_({0, 0})
    , vertexOriginalPosition_({0, 0})
{
    resetCamera();
}

void Canvas::update(){
    updateHoveredItem();
    updateMouseActions();
}

void Canvas::draw() const{
    BeginMode2D(canvasCamera_); {
        drawGrid();
        drawLink();
        Application::instance().graph().draw();
        drawSelect();
    } EndMode2D();
}