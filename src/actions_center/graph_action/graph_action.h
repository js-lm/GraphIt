#pragma once

#include "../action_base_class.h"

struct Vector2;
struct Color;

namespace Action{

    class GraphRelated : public ActionBaseClass{
    public:
        GraphRelated() = default;
        virtual ~GraphRelated() = 0;

        virtual void execute() override = 0;

        virtual void undo() override = 0;
        virtual void redo() override = 0;

    protected:
        void addVertex(Vector2 position, Color color);
        void removeVertex(Vector2 position);
        void removeVertex(size_t id);



    };

    class AddVertex : public GraphRelated{
    public:
        AddVertex(Vector2 position, Color color);
        ~AddVertex() = default;
    };

} // namespace Action