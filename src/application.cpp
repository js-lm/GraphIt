#include "application.h"
#include "graph/graph.h"
#include "actions_center/actions_center.h"
#include "gui/gui.h"
#include "canvas/canvas.h"

#define RAYGUI_IMPLEMENTATION
#include "lib/raygui.h"

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
    delete canvas_;
}

int Application::run(){
    const int screenWidth{1000};
    const int screenHeight{720};

    // SetConfigFlags(FLAG_WINDOW_RESIZABLE); // | FLAG_WINDOW_TRANSPARENT);

    InitWindow(screenWidth, screenHeight, "GraphIt!"); //  v0.1.0
    // SetTargetFPS(300);

    // SetExitKey(KEY_NULL);

    while(!WindowShouldClose()){
        ui_->update();
        canvas_->update();
        actionsCenter_->update();

        BeginDrawing(); {
            ClearBackground(RAYWHITE); // BLANK);

            canvas_->draw();
            ui_->draw();
            
        } EndDrawing();
    }

    CloseWindow();

    return 0;
}
