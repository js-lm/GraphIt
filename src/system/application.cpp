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

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "GraphIt! v" GRAPHIT_VERSION_STRING);
    printInitMessage();

    SetTargetFPS(300);
    SetExitKey(KEY_NULL);
    SetWindowMinSize(screenWidth, screenHeight);
    SetWindowMaxSize(screenWidth * 10, screenHeight * 10);  

    handleWindowResizeEvent(); // since the window was created after the ui

    while(!WindowShouldClose()){
        update();
        ui_->update();
        canvas_->update();
        actionsCenter_->update();

        BeginDrawing(); {
            ClearBackground(RAYWHITE);
            canvas_->draw();
            ui_->draw();
        } EndDrawing();
    }

    CloseWindow();

    return 0;
}

void Application::update(){
    if(IsWindowResized()) handleWindowResizeEvent();
}

void Application::handleWindowResizeEvent(){
    ui_->updatePanelAnchors();
}