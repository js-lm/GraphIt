#include "canvas.h"
#include "system/application.h"
#include "graph/graph.h"
#include "lib/magic_enum.hpp"
#include "system/terminal_prefix.h"

#include <raylib.h>
#include <iostream>
#include <cmath>
#include <raymath.h>
#include <raygui.h>
#include <string>

void Canvas::resetCamera(){
    canvasCamera_.target = {-GetScreenWidth() / 2.0f, -GetScreenHeight() / 2.0f};
    canvasCamera_.offset = {0.0f, 0.0f};
    canvasCamera_.rotation = 0.0f;
    canvasCamera_.zoom = 1.0f;
    zoomFactorTemp_ = 1.0f;
}

void Canvas::drawGrid() const{
    if(!Application::getValue<Setting, bool>(Setting::GRID_SHOW)) return;

    auto canvasPosition{canvasCamera_.target};
    int cellSize{Application::getValue<Setting, int>(Setting::GRID_CELL_SIZE)};
    int lineInterval{Application::getValue<Setting, int>(Setting::GRID_SUBDIVISION_SIZE)};
    int screenHeight{static_cast<int>(GetScreenHeight() / canvasCamera_.zoom)};
    int screenWeight{static_cast<int>(GetScreenWidth() / canvasCamera_.zoom)};
    float lineThickness{1.0f / canvasCamera_.zoom};

    int leftMostVerticalLine{static_cast<int>(canvasPosition.x - static_cast<int>(canvasPosition.x) % cellSize)};
    int verticalLineNumber{static_cast<int>(screenWeight / cellSize) + 1};

    for(auto i{0}; i <= verticalLineNumber; i++){
        float x{static_cast<float>(leftMostVerticalLine + cellSize * i)};

        DrawLineEx(
            {x, canvasCamera_.target.y},
            {x, canvasCamera_.target.y + screenHeight},
            lineThickness * (static_cast<int>(x) % (cellSize * lineInterval) == 0 ? 2.0f : 1.0f),
            GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_DISABLED))
        );
    }

    int topMostHorizontalLine{static_cast<int>(canvasPosition.y - static_cast<int>(canvasPosition.y) % cellSize)};
    int horizontalLineNumber{static_cast<int>(screenHeight / cellSize) + 1};

    for(auto i{0}; i <= horizontalLineNumber; i++){
        float y{static_cast<float>(topMostHorizontalLine + cellSize * i)};

        DrawLineEx(
            {canvasCamera_.target.x, y},
            {canvasCamera_.target.x + screenWeight, y},
            lineThickness * (static_cast<int>(y) % (cellSize * lineInterval) == 0 ? 2.0f : 1.0f),
            GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_DISABLED))
        );
    }
}

void Canvas::drawZoomProgressBar() const{
    if(!lostScrollCD_) return;

    float zoomPercent{(std::log(canvasCamera_.zoom / .2f) / std::log(10.0f))};

    Rectangle progressBarRectangle{
        (GetScreenWidth() - 400) / 2.0f,
        GetScreenHeight() - 40.0f,
        400.0f,
        16.0f
    };

    DrawRectangleRec(progressBarRectangle, Fade(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)), .5f));

    GuiProgressBar(
        progressBarRectangle,
        "20%", 
        "200%", 
        &zoomPercent, 
        0.0f, 
        1.0f
    );

    DrawLine(
        progressBarRectangle.x + 280.0f,
        progressBarRectangle.y + 2.0f,
        progressBarRectangle.x + 280.0f,
        progressBarRectangle.y + 14.0f,
        GetColor(GuiGetStyle(
            DEFAULT, 
            LINE_COLOR
        ))
    );

    char zoomLabel[32];
    std::snprintf(zoomLabel, 32, "#42#Zoom: %.2f", canvasCamera_.zoom);

    GuiLabel(
        {
            progressBarRectangle.x, 
            progressBarRectangle.y - 24, 
            120, 
            24
        }, 
        zoomLabel
    );
}

void Canvas::drawMouse() const{
    if(!IsCursorOnScreen()) return;
    switch(mode_){
    case Mode::VIEW:                    break;
    case Mode::SELECT:  drawSelect();   break;
    case Mode::PAN:                     break;
    case Mode::PEN:     drawPen();      break;
    // case Mode::LINK:                    break;
    case Mode::MOVE:    drawMove();     break;
    case Mode::ERASER:  drawEraser();   break;
    default: break;
    }
}

void Canvas::updateCursor() const{
    if(hoveredVertexID_ || hoveredEdgeIDs_){
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }else{
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
}

void Canvas::drawPen() const{
    if(!isMouseOnCanvas()) return;

    bool snapToGrid{Application::getValue<Setting, bool>(Setting::GRID_IS_SNAP_TO_GRID)};
    Color penColor{Application::getValue<Setting, Color>(Setting::COLOR_DEBUG_PEN)};
    DrawCircleV(
        getMousePositionInCanvas(snapToGrid), 
        Application::instance().getValue<Setting, float>(Setting::GRAPH_VERTEX_RADIUS), 
        penColor
    );
}

void Canvas::drawLink() const{
    if(mode_ != Mode::LINK) return;

    if(linkFrom_){
        auto startingPosition{Application::instance().graph().getVertexPosition(linkFrom_.value())};
        auto thickness{Application::instance().getValue<Setting, float>(Setting::GRAPH_EDGE_THICKNESS)};
        Color linkColor{Application::getValue<Setting, Color>(Setting::COLOR_DEBUG_LINK)};
        DrawLineEx(
            startingPosition,
            getMousePositionInCanvas(),
            thickness,
            linkColor
        );
        DrawCircleV(getMousePositionInCanvas(), thickness / 2, linkColor);
    }

    updateCursor();
}

void Canvas::drawEraser() const{
    updateCursor();
}

void Canvas::drawSelect() const{
    updateCursor();

    if(!startFrom_) return;

    Rectangle rectangle{normalizeRectangle(startFrom_.value(), getMousePositionInCanvas())};

    DrawRectangleRec(rectangle, Fade(BLUE, .5f));
    DrawRectangleLinesEx(rectangle, 2.0f / canvasCamera_.zoom, BLUE);
}

void Canvas::drawMove() const{
    if(hoveredVertexID_ || hoveredEdgeIDs_){
        if(vertexToDrag_){
            SetMouseCursor(MOUSE_CURSOR_RESIZE_ALL);
        }else{
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        }
    }else{
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
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
    if(!Application::instance().ui().isMouseOnCanvas()
    || mode_ == Mode::PAN
    ){
        hoveredVertexID_ = std::nullopt;
        hoveredEdgeIDs_ = std::nullopt;
        return;
    }

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
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
}

Rectangle Canvas::normalizeRectangle(Vector2 startingPosition, Vector2 endingPosition) const{
    float left{std::min(startingPosition.x, endingPosition.x)};
    float top{std::min(startingPosition.y, endingPosition.y)};
    float width{std::abs(startingPosition.x - endingPosition.x)};
    float height{std::abs(startingPosition.y - endingPosition.y)};

    return {left, top, width, height};
}