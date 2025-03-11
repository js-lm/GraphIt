#pragma once

#include <memory>
#include <utility>
#include <vector>

class ActionBaseClass{
public:
    ActionBaseClass() = default;
    virtual ~ActionBaseClass() = default;
    virtual void execute() = 0;

    virtual void undo() = 0;
    virtual void redo() = 0;
};

class ActionsCenter{
public:
    using UniqueAction = std::unique_ptr<ActionBaseClass>;

public:
    struct StackIndex{
        size_t current;
        size_t total;
    };

public:
    static std::unique_ptr<ActionsCenter> createActionsCenter();

public:
    void addAction(std::unique_ptr<ActionBaseClass> action){ actionsToExecute_.emplace_back(std::move(action));};

    // move actions from queue to stack and execute them
    void update();

    void undo();
    void redo();

    bool canUndo() const{ return currentActionIndex_ > 0;};
    bool canRedo() const{ return currentActionIndex_ + 1 < actionsStack_.size();};

    const StackIndex getStackIndex() const{ return {currentActionIndex_, actionsStack_.size()};};

private:
    ActionsCenter() = default;
    ~ActionsCenter() = default;

private:
    // actions history
    std::vector<UniqueAction> actionsStack_;
    size_t currentActionIndex_;

    // newly added actions to execute in this frame
    std::vector<UniqueAction> actionsToExecute_;
};