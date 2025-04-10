#include "actions_center.hpp"
#include "system/terminal_prefix.hpp"
#include "system/settings.hpp"
#include "system/application.hpp"
#include "canvas/canvas.hpp"

void ActionsCenter::initAlgorithm(){
    exitAlgorithm();
    previousMode_ = appSettings.toolbarCurrentSelectedTool;
    Application::instance().canvas().switchMode(Canvas::Mode::PAN);
    appFlags.algorithmFocusMode = true;
    algorithmStepsStack_.emplace_back(std::make_unique<Action::DUMMY>());
}

void ActionsCenter::algorithmDo(UniqueAction &&action){
    std::cout << "Step [" << currentStepIndex_ << "] " 
              << action->getName() << std::endl;
    action->execute();
    algorithmStepsStack_.emplace_back(std::move(action));
    currentStepIndex_ = algorithmStepsStack_.size() - 1;
}

void ActionsCenter::previousStep(){
    if(!canStepBackward()) return;
    auto &step{algorithmStepsStack_[currentStepIndex_--]};
    printAlgorithmPrefix();
    std::cout << "Step [" << currentStepIndex_ << "/" 
              << algorithmStepsStack_.size() - 1 << "] " 
              << step->getName() << std::endl;
    step->undo();
    updateStepsAvailability();
}

void ActionsCenter::nextStep(){
    if(!canStepForward()) return;
    auto &step{algorithmStepsStack_[++currentStepIndex_]};
    printAlgorithmPrefix();
    std::cout << "Step [" << currentStepIndex_ << "/" 
              << algorithmStepsStack_.size() - 1 << "] " 
              << step->getName() << std::endl;
    step->redo();
    updateStepsAvailability();
    if(!canStepForward()){
        appFlags.algorithmIsRunning = false;
    }
}

void ActionsCenter::updateStepsAvailability(){
    appFlags.algorithmCanStepBackward = canStepBackward();
    appFlags.algorithmCanStepForward = canStepForward();
}

void ActionsCenter::exitAlgorithm(){
    while(!algorithmStepsStack_.empty()){
        algorithmStepsStack_.back()->undo();
        algorithmStepsStack_.pop_back();
    }
    currentStepIndex_ = 0;
    appFlags.algorithmFocusMode = false;
    appFlags.algorithmCanStepBackward = true;
    appFlags.algorithmCanStepForward = true;
    Application::instance().canvas().switchMode(static_cast<Canvas::Mode>(previousMode_));
}

void ActionsCenter::backToFirstStep(){
    while(canStepBackward()){
        algorithmStepsStack_[currentStepIndex_--]->undo();
    }
}