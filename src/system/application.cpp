#include "system/version.h"
#include "system/application.h"
#include "graph/graph.h"
#include "actions_center/actions_center.h"
#include "ui/ui.h"
#include "canvas/canvas.h"
#include "system/terminal_prefix.h"
#include "io/io.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <raylib.h>
#include <iostream>

Application::Application()
    : graph_(new Graph())
    , actionsCenter_(new ActionsCenter())
    , ui_(new UI::Center())
    , canvas_(new Canvas())
    , serializer_(new Serializer())
{
    initSettings();
    initFlags();
}

Application::~Application(){
    delete graph_;
    delete actionsCenter_;
    delete ui_;
    delete canvas_;
    delete serializer_;
}

int Application::run(){
    printInitMessage(GRAPHIT_VERSION_STRING);
    
    const int screenWidth{1000};
    const int screenHeight{720};

    // SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_TRANSPARENT);

    InitWindow(screenWidth, screenHeight, "GraphIt! v" GRAPHIT_VERSION_STRING);
    // SetTargetFPS(300);

    printInitMessage();

    // SetExitKey(KEY_NULL);

    while(!WindowShouldClose()){
        ui_->update();
        canvas_->update();
        actionsCenter_->update();

        BeginDrawing(); {
            ClearBackground(RAYWHITE); // BLANK);

            canvas_->draw();
            ui_->draw();
            
            DrawFPS(screenWidth - 50, screenHeight - 20);
        } EndDrawing();
    }

    CloseWindow();

    return 0;
}
