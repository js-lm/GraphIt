#include "canvas.h"
#include "../application.h"
#include "../graph/graph.h"

#include <raylib.h>
#include <iostream>

Canvas::Canvas()
    : mode_(Mode::VIEW)
{
    resetCamera();
}

void Canvas::update(){
    updateKeyboardActions();
    updateHoveredItem();
    updateMouseActions();

    if(hoveredVertexID_){
        std::cout << hoveredVertexID_.value() << std::endl;
    }
}

void Canvas::draw() const{
    BeginMode2D(canvasCamera_); {
        drawGrid();
        Application::instance().graph().draw();
    } EndMode2D();
}