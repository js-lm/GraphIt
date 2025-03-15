#include "actions_center.h"
#include "action_base_class.h"

#include <iostream>

void ActionsCenter::update(){
    if(actionsToExecute_.empty()) return;

    std::cout << "Updating ActionsCenter\n";
    std::cout << "BEFORE: actionsStack_.size(): " << actionsStack_.size() << std::endl;

    if(canRedo()){
        actionsStack_.erase(actionsStack_.begin() + currentActionIndex_ + 1, actionsStack_.end());
    }

    actionsStack_.reserve(actionsStack_.size() + actionsToExecute_.size());

    for(auto& action : actionsToExecute_){
        action->execute();
        if(action->shouldSave()){
            actionsStack_.push_back(std::move(action));
        }
    }

    actionsToExecute_.clear();
    currentActionIndex_ = actionsStack_.size() - 1;

    std::cout << "AFTER: actionsStack_.size(): " << actionsStack_.size() << std::endl;
}

void ActionsCenter::undo(){ 
    if(!canUndo()) return;
    actionsStack_[currentActionIndex_--]->undo();
};

void ActionsCenter::redo(){ 
    if(!canRedo()) return;
    actionsStack_[++currentActionIndex_]->redo();
};