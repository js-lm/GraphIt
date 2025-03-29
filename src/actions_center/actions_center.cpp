#include "actions_center.hpp"
#include "action_base_class.hpp"
#include "system/terminal_prefix.hpp"

#include <iostream>

ActionsCenter::ActionsCenter(){
    actionsStack_.emplace_back(std::make_unique<Action::DUMMY>());
}

void ActionsCenter::update(){
    moveActionsFromQueueToStack();
}

void ActionsCenter::moveActionsFromQueueToStack(){
    if(actionsToExecute_.empty()) return;

    if(canRedo()){
        actionsStack_.erase(actionsStack_.begin() + currentActionIndex_ + 1, actionsStack_.end());
    }

    actionsStack_.reserve(actionsStack_.size() + actionsToExecute_.size());

    for(auto &action : actionsToExecute_){
        action->execute();
        printActionPrefix();
        std::cout << "Executing " << action->getName();
        if(action->shouldSave()){
            actionsStack_.emplace_back(std::move(action));
            std::cout << " (Recorded)";
        }

        std::cout << std::endl;
    }

    actionsToExecute_.clear();
    currentActionIndex_ = actionsStack_.size() - 1;
}

void ActionsCenter::undo(){ 
    if(!canUndo()) return;
    auto &action{actionsStack_[currentActionIndex_--]};
    printActionPrefix();
    std::cout << "Undoing " << action->getName() << " ";
    printCurrentIndex();
    std::cout << std::endl;
    action->undo();
}

void ActionsCenter::redo(){ 
    if(!canRedo()) return;
    auto &action{actionsStack_[++currentActionIndex_]};
    printActionPrefix();
    std::cout << "Redoing " << action->getName() << " ";
    printCurrentIndex();
    std::cout << std::endl;
    action->redo();
}

void ActionsCenter::printCurrentIndex() const{
    std::cout << currentActionIndex_ << "/" << actionsStack_.size() - 1;
}

void ActionsCenter::clearHistory(){
    actionsStack_.clear();
    currentActionIndex_ = 0;
    actionsToExecute_.clear();
    actionsStack_.emplace_back(std::make_unique<Action::DUMMY>());
}