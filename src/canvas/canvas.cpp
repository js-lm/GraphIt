#include "canvas.hpp"
#include "system/application.hpp"
#include "graph/graph.hpp"

#include <raylib.h>

Canvas::Canvas(float dpiScaling)
    : mode_(Mode::VIEW)
    , dpiScaling_{dpiScaling}
    , pivotPoint_({0, 0})
    , vertexOriginalPosition_({0, 0})
{}

void Canvas::update(){
    updateHoveredItem();
    updateMouseActions();
}

void Canvas::draw() const{
    BeginMode2D(canvasCamera_); {
        drawGrid();
        drawLink();
        Application::instance().graph().draw();
        drawMouse();
    } EndMode2D();

    drawMouseLabel();

    drawZoomProgressBar();
}