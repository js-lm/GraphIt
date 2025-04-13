#include "system/version.hpp"
#include "system/application.hpp"
#include "graph/graph.hpp"
#include "actions_center/actions_center.hpp"
#include "ui/ui.hpp"
#include "canvas/canvas.hpp"
#include "system/terminal_prefix.hpp"
#include "io/io.hpp"
#include "icon/icon.h"
#include "algorithms/algorithm_center.hpp"

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#include <raylib.h>
#include <raymath.h>
#include <iostream>

Application::Application()
    : graph_(new Graph())
    , actionsCenter_(new ActionsCenter())
    , ui_(new UI::Center())
    , canvas_(new Canvas())
    , serializer_(new Serializer())
    , algorithm_(new AlgorithmCenter())
{}

Application::~Application(){
    delete graph_;
    delete actionsCenter_;
    delete ui_;
    delete canvas_;
    delete serializer_;
    delete algorithm_;
}

void Application::init(){
    printInitMessage(GRAPHIT_VERSION_STRING);

    const int screenWidth{1000};
    const int screenHeight{720};

    previousWindowRectangle_ = {screenWidth, screenHeight};

    // SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "GraphIt! v" GRAPHIT_VERSION_STRING);

    SetTargetFPS(300);
    SetExitKey(KEY_NULL);
    SetWindowMinSize(screenWidth, screenHeight);
    SetWindowMaxSize(screenWidth * 10, screenHeight * 10);

    SetWindowIcon(Image((void*)ICON_DATA, ICON_WIDTH, ICON_HEIGHT, 1, ICON_FORMAT));

    handleWindowResizeEvent(); // since the window was created after the ui
    canvas_->resetCamera();

    printInitMessage();
}

int Application::run(int argumentsCount, char* arguments[]){
    if(handleArguments(argumentsCount, arguments)) return 0;
    init();
    while(!WindowShouldClose()){
        update();
        BeginDrawing();
        draw();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

void Application::update(){
    ui_->update();
    canvas_->update();
    actionsCenter_->update();
    algorithm_->update();

    if(IsWindowResized()) handleWindowResizeEvent();
}

void Application::draw(){
    ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
    canvas_->draw();
    ui_->draw();
    // DrawFPS(0, GetScreenHeight() - 16);
}

void Application::handleWindowResizeEvent(){
    ui_->updatePanelAnchors();

    Vector2 delta{
        previousWindowRectangle_.x - GetScreenWidth(), 
        previousWindowRectangle_.y - GetScreenHeight()
    };

    Vector2 cameraPosition{canvas_->getCameraPosition()};
    Vector2 cameraNewPosition{
        Vector2Add(
            cameraPosition, 
            Vector2Scale(delta, .5f / canvas_->getCameraZoom())
        )
    };

    canvas_->setCameraPosition(cameraNewPosition);

    previousWindowRectangle_ = {
        static_cast<float>(GetScreenWidth()), 
        static_cast<float>(GetScreenHeight())
    };
}

bool Application::handleArguments(int argumentsCount, char* arguments[]){
    for(int i{1}; i < argumentsCount; i++){
        std::string argument{arguments[i]};
        if(argument == "--version"){
            std::cout << "GraphIt! version " << GRAPHIT_VERSION_STRING << std::endl;
        }else if(argument == "--help"){

        }else if(argument == "--credit"){
            std::cout << "Author: js-lm\n"
                      << "Release: https://github.com/js-lm/GraphIt"
                      << std::endl;
        }else{
            std::cout << "Unknown argument '" << argument 
                      << "'. Use '--help' for usage instructions." << std::endl;;
        }
    }

    return argumentsCount > 1;
}