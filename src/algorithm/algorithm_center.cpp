#include "algorithm_center.hpp"
#include "system/settings.hpp"
#include "BFS/bfs.hpp"
#include "system/application.hpp"
#include "actions_center/actions_center.hpp"

#include <raylib.h>
#include <iostream>
#include <cmath>

// using namespace Algorithm;

void AlgorithmCenter::update(){
    if(!appFlags.algorithmFocusMode 
    || !appSettings.algorithmIsAutoForward
    || !appFlags.algorithmIsRunning
    ){
        return;
    }

    // I don't know why but the program freezes when it's not static
    static float autoClock{0.0f};
    
    // std::cout << "update() called- autoForwardSecondPerStep " << appSettings.autoForwardSecondPerStep << std::endl;
    
    float frameTime{GetFrameTime()};
    // std::cout << "GetFrameTime() " << frameTime << std::endl;
    
    if(autoClock >= appSettings.autoForwardSecondPerStep){
        autoClock = 0.0f;
        // std::cout << "autoClock reset" << std::endl;
        nextStep();
    }else{
        autoClock += frameTime;
        // std::cout << "autoClock + frameTime " << std::endl;
    }
    
    // std::cout << "update() exits" << std::endl;
}

void AlgorithmCenter::run(){
    appFlags.algorithmIsRunning = true;
    appFlags.algorithmFocusMode = true;
    Application::instance().actionCenter().initAlgorithm();
    Algorithm::BFS bfs(0);
    Application::instance().actionCenter().backToFirstStep();
    // std::cout << "run() exits" << std::endl;
}

void AlgorithmCenter::exit(){
    appFlags.algorithmIsRunning = false;
    appFlags.algorithmFocusMode = false;
    Application::instance().actionCenter().exitAlgorithm();
}

void AlgorithmCenter::previousStep(){
    Application::instance().actionCenter().previousStep();
}

void AlgorithmCenter::nextStep(){
    Application::instance().actionCenter().nextStep();
}
