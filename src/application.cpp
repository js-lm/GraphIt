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
    const int screenWidth{800};
    const int screenHeight{600};

    InitWindow(screenWidth, screenHeight, "Graph Test");
    // SetTargetFPS(300);

    while(!WindowShouldClose()){

        canvas_->update();
        actionsCenter_->update();

        BeginDrawing(); {
            ClearBackground(RAYWHITE);
            
            canvas_->draw();
            ui_->drawToolbar();
            
        } EndDrawing();
    }

    CloseWindow();
}
