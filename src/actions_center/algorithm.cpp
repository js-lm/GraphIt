#include "actions_center.hpp"
#include "system/terminal_prefix.hpp"
#include "system/settings.hpp"

void ActionsCenter::initAlgorithm(){
    exitAlgorithm();
    appFlags.algorithmFocusMode = true;
    algorithmStepsStack_.emplace_back(std::make_unique<Action::DUMMY>());
}

void ActionsCenter::algorithmDo(UniqueAction &&action){
    std::cout << "\t> Step [" << currentStepIndex_ << "] " 
              << action->getName() << std::endl;
    action->execute();
    if(action->shouldSave()){
        algorithmStepsStack_.emplace_back(std::move(action));
    }
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
}

void ActionsCenter::nextStep(){
    if(!canStepForward()) return;
    auto &step{algorithmStepsStack_[++currentStepIndex_]};
    printAlgorithmPrefix();
    std::cout << "Step [" << currentStepIndex_ << "/" 
              << algorithmStepsStack_.size() - 1 << "] " 
              << step->getName() << std::endl;
    step->redo();
}

void ActionsCenter::exitAlgorithm(){
    while(!algorithmStepsStack_.empty()){
        algorithmStepsStack_.back()->undo();
        algorithmStepsStack_.pop_back();
    }
    currentStepIndex_ = 0;
    appFlags.algorithmFocusMode = false;
}

void ActionsCenter::backToFirstStep(){
    printAlgorithmPrefix();
    std::cout << "Back to fist step" << std::endl;
    while(canStepBackward()){
        previousStep();
        std::cout << "stepping backward" << std::endl;
    }
}