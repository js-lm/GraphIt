#include "application.h"
#include "graph/graph.h"
#include "actions_center/actions_center.h"
#include "gui/gui.h"
#include "canvas/canvas.h"

#include <raylib.h>

Application::Application()
    : graph_(new Graph())
    , actionsCenter_(new ActionsCenter())
    , ui_(new GUI())
    , canvas_(new Canvas())
{}

Application::~Application(){
    delete graph_;
    delete actionsCenter_;
    delete ui_;
}

void Application::run(){
    const int screenWidth{1000};
    const int screenHeight{720};

    SetConfigFlags(FLAG_WINDOW_RESIZABLE); // | FLAG_WINDOW_TRANSPARENT);

    InitWindow(screenWidth, screenHeight, "Graph Test");
    // SetTargetFPS(300);

    while(!WindowShouldClose()){
        ui_->update();
        canvas_->update();
        actionsCenter_->update();

        BeginDrawing(); {
            ClearBackground(RAYWHITE);
            // ClearBackground(BLANK);

            canvas_->draw();
            ui_->draw();
            
        } EndDrawing();
    }

    CloseWindow();
}
