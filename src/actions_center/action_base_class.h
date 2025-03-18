#pragma once

#include "lib/magic_enum.hpp"

#include <string>

class ActionBaseClass{
protected:
    enum class ID{
        DUMMY,

        ADD_VERTEX,
        REMOVE_VERTEX,
        CONNECT_VERTICES,
        DISCONNECT_VERTICES,
        MOVE_VERTEX,
        BULK_REMOVE,
        BULK_REMOVE_VERTICES,
        BULK_REMOVE_EDGES,
        DYE_VERTEX,
        DYE_EDGE,
        DYE
    };

public:
    ActionBaseClass() : shouldBeRecorded_(false), identifier_(ID::DUMMY){};
    virtual ~ActionBaseClass() = default;
    virtual void execute() = 0;

    virtual void undo() = 0;
    virtual void redo() = 0;

    std::string getName() const{ return std::string(magic_enum::enum_name(identifier_));};

    // should the action be recorded
    const bool shouldSave() const{ return shouldBeRecorded_;};

protected:
    bool shouldBeRecorded_;
    
    ID identifier_;
};