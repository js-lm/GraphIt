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
    ActionsCenter();
    ~ActionsCenter() = default;

    void addAction(UniqueAction&& action){ actionsToExecute_.emplace_back(std::move(action));};

    void update();

    void undo();
    void redo();

    bool canUndo() const{ return currentActionIndex_ > 0;}
    bool canRedo() const{ return currentActionIndex_ + 1 < actionsStack_.size();}

    const StackIndex getStackIndex() const{ return {currentActionIndex_, actionsStack_.size()};}

    void clearHistory();

private:
    void moveActionsFromQueueToStack();

    void printCurrentIndex() const;

private:
    // actions history
    std::vector<UniqueAction> actionsStack_;
    size_t currentActionIndex_{0};

    // newly added actions to execute in this frame
    std::vector<UniqueAction> actionsToExecute_;
};

namespace Action{
    
// the bottom action
class DUMMY : public ActionBaseClass{
public:
    DUMMY(){ 
        shouldBeRecorded_ = true;
        identifier_ = ID::DUMMY;
    };
    ~DUMMY() = default;

    void execute() override{};

    void undo() override{};
    void redo() override{};
};

} // namespace Action