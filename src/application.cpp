#include "application.h"
#include "graph/graph.h"
#include "actions_center/actions_center.h"
#include "gui/gui.h"

#include <raylib>

Application::Application()
    : graph_(new Graph())
    , actionsCenter_(new ActionsCenter())
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


        
        BeginDrawing(); {
            ClearBackground(RAYWHITE);
            

            ui_->drawToolbar();
            
        } EndDrawing();
    }

    CloseWindow();
}
