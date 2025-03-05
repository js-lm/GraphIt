#include "canvas.h"
#include "lib/magic_enum.hpp"

#include <functional>
#include <raymath.h>

Canvas::Canvas()
    : graph_(new Graph(this))
    , canvasCamera_({})
    , isViewing_(false)
{
    canvasCamera_.target = {0, 0};
    canvasCamera_.offset = {0, 0};
    canvasCamera_.zoom = 1.0f;
}

Canvas::~Canvas(){
    delete graph_;
}

void Canvas::update(){
    graph_->update();
}

void Canvas::drawTools(){
    Rectangle bounds{11, 5, 70, 20};
    float gap{25};

    auto drawButton{
        [&](const char *label, std::function<void()> action){
            bounds.y += gap;
            if(GuiButton(bounds, label)) action();
        }
    };

    changeButtonStyleTo(TOGGLE, 0x4A5568FF, 0x2D3748FF, 0xFFFFFFFF, 2);
    GuiToggle({5, 5, 82, 25}, (isViewing_ ? "#120#" : "#121#"), &isViewing_);

    bounds.y += 5;

    if(!isViewing_){
        DrawRectangleLinesEx({5, 5, 82, 31 + 7 * gap}, 2, colorFromHex(0x4A5568FF));

        changeButtonStyleTo(BUTTON, 0x2B6CB0FF, 0x4299E1FF, 0xFFFFFFFF, 2);
        drawButton("Add", [&](){ switchMode(Graph::Mode::ADD);});
        drawButton("Connect", [&](){ switchMode(Graph::Mode::CONNECT);});
        
        changeButtonStyleTo(BUTTON, 0x319795FF, 0x38B2ACFF, 0xFFFFFFFF, 2);
        drawButton("Drag", [&](){ switchMode(Graph::Mode::DRAG);});

        changeButtonStyleTo(BUTTON, 0xDD6B20FF, 0xED8936FF, 0xFFFFFFFF, 2);
        drawButton("Delete Node", [&](){ switchMode(Graph::Mode::DEL_NODE);});
        drawButton("Delete Edge", [&](){ switchMode(Graph::Mode::DEL_EDGE);});

        changeButtonStyleTo(BUTTON, 0xC53030FF, 0xE53E3EFF, 0xFFFFFFFF, 2);
        drawButton("Reset", [&](){ 
            graph_->reset();
            switchMode(Graph::Mode::EDIT);
        });

        changeButtonStyleTo(BUTTON, 0x00000000, 0x00000000, 0x000000FF, 2);
        drawButton(magic_enum::enum_name(graph_->getMode()).data(), [&](){
            switchMode(Graph::Mode::EDIT);
        });
    }
}

void Canvas::drawGraph() const{
    BeginMode2D(canvasCamera_); {
        graph_->draw();
    } EndMode2D();
}

void Canvas::switchMode(Graph::Mode mode){
    graph_->switchMode(mode);
}

void Canvas::changeButtonStyleTo(GuiControl target, int border, int base, int text, int borderWidth){
    auto lighten{
        [](int color, float factor){
            int r{std::min(255, static_cast<int>(((color >> 24) & 0xFF) + (255 - ((color >> 24) & 0xFF)) * factor))};
            int g{std::min(255, static_cast<int>(((color >> 16) & 0xFF) + (255 - ((color >> 16) & 0xFF)) * factor))};
            int b{std::min(255, static_cast<int>(((color >> 8) & 0xFF) + (255 - ((color >> 8) & 0xFF)) * factor))};
            return (r << 24) | (g << 16) | (b << 8) | (color & 0xFF);
        }
    };

    auto darken{
        [](int color, float factor){
            int r{std::max(0, static_cast<int>(((color >> 24) & 0xFF) * (1 - factor)))};
            int g{std::max(0, static_cast<int>(((color >> 16) & 0xFF) * (1 - factor)))};
            int b{std::max(0, static_cast<int>(((color >> 8) & 0xFF) * (1 - factor)))};
            return (r << 24) | (g << 16) | (b << 8) | (color & 0xFF);
        }
    };

    GuiSetStyle(target, BORDER_WIDTH, borderWidth);

    GuiSetStyle(target, BORDER_COLOR_NORMAL, border);
    GuiSetStyle(target, BASE_COLOR_NORMAL, base);
    GuiSetStyle(target, TEXT_COLOR_NORMAL, text);

    float BUTTON_HOVER_LIGHTEN_FACTOR{.3f};
    float BUTTON_PRESSED_DARKEN_FACTOR{.3f};

    GuiSetStyle(target, BORDER_COLOR_FOCUSED, lighten(border, BUTTON_HOVER_LIGHTEN_FACTOR));
    GuiSetStyle(target, BASE_COLOR_FOCUSED, lighten(base, BUTTON_HOVER_LIGHTEN_FACTOR));
    GuiSetStyle(target, TEXT_COLOR_FOCUSED, lighten(text, BUTTON_HOVER_LIGHTEN_FACTOR));

    GuiSetStyle(target, BORDER_COLOR_PRESSED, darken(border, BUTTON_PRESSED_DARKEN_FACTOR));
    GuiSetStyle(target, BASE_COLOR_PRESSED, darken(base, BUTTON_PRESSED_DARKEN_FACTOR));
    GuiSetStyle(target, TEXT_COLOR_PRESSED, darken(text, BUTTON_PRESSED_DARKEN_FACTOR));
}

Color Canvas::colorFromHex(unsigned int hexValue){
    return (Color){
        (unsigned char)((hexValue & 0xFF000000) >> 24),
        (unsigned char)((hexValue & 0x00FF0000) >> 16),
        (unsigned char)((hexValue & 0x0000FF00) >> 8),
        (unsigned char)(hexValue & 0x000000FF)
    }; 
}

void Canvas::updateCanvasCamera() {
    if(IsKeyPressed(KEY_R)
    || IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON)){
        canvasCamera_.target = {0, 0};
        canvasCamera_.offset = {0, 0};
        canvasCamera_.zoom = 1.0f;
    }
    
    float wheel{GetMouseWheelMove()};
    if(wheel != 0){
        Vector2 mouseCanvasPosition{GetScreenToWorld2D(GetMousePosition(), canvasCamera_)};
        
        canvasCamera_.zoom += wheel * 0.1f * canvasCamera_.zoom;
        canvasCamera_.zoom = Clamp(canvasCamera_.zoom, 0.5f, 2.0f);
        
        Vector2 newMouseCanvasPosition{GetScreenToWorld2D(GetMousePosition(), canvasCamera_)};
        canvasCamera_.target.x += mouseCanvasPosition.x - newMouseCanvasPosition.x;
        canvasCamera_.target.y += mouseCanvasPosition.y - newMouseCanvasPosition.y;
    }
    
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        canvasCameraPivot_ = GetMousePosition();
    }
    
    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
        Vector2 delta{
            (canvasCameraPivot_.x - GetMousePosition().x) / canvasCamera_.zoom,
            (canvasCameraPivot_.y - GetMousePosition().y) / canvasCamera_.zoom
        };
        
        canvasCamera_.target.x += delta.x;
        canvasCamera_.target.y += delta.y;
        canvasCameraPivot_ = GetMousePosition();
    }
}