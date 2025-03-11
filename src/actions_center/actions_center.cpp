#include "actions_center.h"

void ActionsCenter::update(){
    if(actionsToExecute_.empty()) return;

    if(canRedo()){
        actionsStack_.erase(actionsStack_.begin() + currentActionIndex_ + 1, actionsStack_.end());
    }

    actionsStack_.reserve(actionsToExecute_.size());

    for(const auto &action : actionsToExecute_){
        action->execute();
        actionsStack_.emplace_back(std::move(action));
    }

    actionsToExecute_.clear();
}

void ActionsCenter::undo(){ 
    if(!canUndo()) return;
    actionsStack_[--currentActionIndex_]->undo();
};

void ActionsCenter::redo(){ 
    if(!canRedo()) return;
    actionsStack_[++currentActionIndex_]->redo();
};