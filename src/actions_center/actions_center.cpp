#include "actions_center.h"
#include "action_base_class.h"

#include <assert.h>

void ActionsCenter::update(){
    if(actionsToExecute_.empty()) return;

    if(canRedo()){
        actionsStack_.erase(actionsStack_.begin() + currentActionIndex_ + 1, actionsStack_.end());
    }

    actionsStack_.reserve(actionsToExecute_.size());

    for(const auto &action : actionsToExecute_){
        action->execute();
        if(action->shouldSave()){
            actionsStack_.emplace_back(std::move(action));
        }
    }

    actionsToExecute_.clear();
    currentActionIndex_ = actionsStack_.size() - 1;
}

void ActionsCenter::undo(){ 
    if(!canUndo()) return;
    actionsStack_[--currentActionIndex_]->undo();
};

void ActionsCenter::redo(){ 
    if(!canRedo()) return;
    actionsStack_[++currentActionIndex_]->redo();
};