#include "canvas.h"
#include "../application.h"
#include "../graph/graph.h"

#include <raylib.h>

Canvas::Canvas()
    : mode_(Mode::VIEW)
    , isMouseDeactivated_(false)
{
    resetCamera();
}

void Canvas::update(){
    if(!isMouseDeactivated_){
        updateHoveredItem();
        updateMouseActions();
    }
}

void Canvas::draw() const{
    BeginMode2D(canvasCamera_); {
        drawGrid();
        Application::instance().graph().draw();
    } EndMode2D();
}