#pragma once

class ActionBaseClass{
public:
    ActionBaseClass() = default;
    virtual ~ActionBaseClass() = default;
    virtual void execute() = 0;

    virtual void undo() = 0;
    virtual void redo() = 0;

    // should the action be added to history stack
    const bool shouldSave() const{ return shouldSaveHistory_;};

protected:
    bool shouldSave_;
};