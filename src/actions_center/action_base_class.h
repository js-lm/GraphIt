#pragma once

class ActionBaseClass{
public:
    ActionBaseClass() : shouldRecorded_(false){};
    virtual ~ActionBaseClass() = default;
    virtual void execute() = 0;

    virtual void undo() = 0;
    virtual void redo() = 0;

    // should the action be recorded
    const bool shouldSave() const{ return shouldRecorded_;};

protected:
    bool shouldRecorded_;
};