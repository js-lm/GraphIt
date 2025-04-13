#include "algorithm_center.hpp"
#include "system/settings.hpp"
#include "BFS/bfs.hpp"
#include "system/application.hpp"
#include "actions_center/actions_center.hpp"
#include "Kruskal/kruskal.hpp"
#include "system/application.hpp"
#include "graph/graph.hpp"
#include "canvas/canvas.hpp"

#include <raylib.h>
#include <iostream>
#include <cmath>

AlgorithmCenter::AlgorithmCenter()
    : hasExecuted_(false)
    , autoClock_(0.0f)
{}

void AlgorithmCenter::update(){
    if(!appFlags.algorithmFocusMode 
    || !appSettings.algorithmIsAutoForward
    || !appFlags.algorithmIsRunning
    ){
        return;
    }

    if(!hasExecuted_){
        switch(appStates.algorithmDropdownOption){
        case 0: // BFS
        case 1:{ // Prim's
            setStartVertex(); 
            if(startVertex_) executeAlgorithm();
            startVertex_ = std::nullopt;
            break;
        }
        
        default: executeAlgorithm(); break;
        }
    }else{
        replayAlgorithm();
    }
}

void AlgorithmCenter::run(){
    appFlags.algorithmIsRunning = true;
    appFlags.algorithmFocusMode = true;
    Application::instance().actionCenter().initAlgorithm();
}

void AlgorithmCenter::setStartVertex(){
    hoveredVertex_ = Application::instance().graph().findVertex(
        Application::instance().canvas().getMousePositionInCanvas(), 
        appSettings.graphVertexRadius * 1.5f
    );

    if(hoveredVertex_ && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        startVertex_ = hoveredVertex_;
    }

    appStates.cursorString = "Please Select A Vertex To Start";
}

void AlgorithmCenter::executeAlgorithm(){
    switch(appStates.algorithmDropdownOption){
    case 0:{ Algorithm::BFS bfs(startVertex_); break;}
    case 1:{ break;}
    case 2:{ Algorithm::Kruskal kruskal; break;}
    default: break;
    }
    hasExecuted_ = true;
    Application::instance().actionCenter().backToFirstStep();

    appStates.cursorString.clear();
}

void AlgorithmCenter::replayAlgorithm(){
    // I don't know why but the program freezes when it's not static
    // static float autoClock{0.0f};
    // Because you didn't declare the class in Application dumb ass
    
    if(autoClock_ >= appSettings.autoForwardSecondPerStep){
        autoClock_ = 0.0f;
        nextStep();
    }else{
        autoClock_ += GetFrameTime();
    }
}

void AlgorithmCenter::exit(){
    appFlags.algorithmIsRunning = false;
    appFlags.algorithmFocusMode = false;
    hasExecuted_ = false;
    autoClock_ = 0.0f;
    Application::instance().actionCenter().exitAlgorithm();
}

void AlgorithmCenter::previousStep(){
    Application::instance().actionCenter().previousStep();
}

void AlgorithmCenter::nextStep(){
    Application::instance().actionCenter().nextStep();
}
