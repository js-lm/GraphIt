#pragma once

#include "action_base_class.h" 

#include <memory>
#include <utility>
#include <vector>

class ActionsCenter{
public:
    using UniqueAction = std::unique_ptr<ActionBaseClass>;

public:
    struct StackIndex{
        size_t current;
        size_t total;
    };

public:
    ActionsCenter() = default;
    ~ActionsCenter() = default;

    void addAction(UniqueAction&& action){ actionsToExecute_.push_back(std::move(action));}

    // move actions from queue to stack and execute them
    void update();

    void undo();
    void redo();

    bool canUndo() const{ return currentActionIndex_ > 0;}
    bool canRedo() const{ return currentActionIndex_ + 1 < actionsStack_.size();}

    const StackIndex getStackIndex() const{ return {currentActionIndex_, actionsStack_.size()};}

private:
    // actions history
    std::vector<UniqueAction> actionsStack_;
    size_t currentActionIndex_{0};

    // newly added actions to execute in this frame
    std::vector<UniqueAction> actionsToExecute_;
};