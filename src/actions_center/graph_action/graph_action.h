#pragma once

#include "../action_base_class.h"

#include <optional>
#include <raylib.h>

namespace Action{

    class GraphRelated : public ActionBaseClass{
    public:
        GraphRelated(){ shouldBeRecorded_ = true;};
        virtual ~GraphRelated() = default;

        virtual void execute() override = 0;

        virtual void undo() override = 0;
        virtual void redo() override = 0;

    protected:
        size_t addVertex(Vector2 position, std::optional<Color> color = std::nullopt);
        bool removeVertex(size_t id);
        void restoreRemovedVertex(size_t id);

        bool connectVertices(size_t startID, size_t endID, std::optional<Color> color = std::nullopt);
        std::optional<Color> disconnectVertices(size_t startID, size_t endID);
    };

    class AddVertex : public GraphRelated{
    public:
        AddVertex(Vector2 position, Color color)
            : position_(position), color_(color){ identifier_ = ID::ADD_VERTEX;};
        ~AddVertex() = default;

        void execute() override{ vertexId_ = addVertex(position_, color_);};

        void undo() override{ removeVertex(vertexId_);};
        void redo() override{ restoreRemovedVertex(vertexId_);};

    private:
        Vector2 position_;
        Color color_;
        size_t vertexId_;
    };

    class RemoveVertex : public GraphRelated{
    public:
        RemoveVertex(size_t id) : vertexID_(id){ identifier_ = ID::REMOVE_VERTEX;};
        ~RemoveVertex() = default;

        void execute() override{ removeVertex(vertexID_);};

        void undo() override{ restoreRemovedVertex(vertexID_);};
        void redo() override{ execute();};

    private:
        size_t vertexID_;
    };

    class ConnectVertices : public GraphRelated{
    public:
        ConnectVertices(size_t startID, size_t endID, std::optional<Color> color = std::nullopt)
            : startID_(startID), endID_(endID)
            , color_(color)
        {
            identifier_ = ID::CONNECT_VERTICES;
        };
        
        void execute() override{ connectVertices(startID_, endID_, color_);};

        void undo() override{ disconnectVertices(startID_, endID_);};
        void redo() override{ execute();};

    private:
        size_t startID_;
        size_t endID_;
        
        std::optional<Color> color_;
    };

    class DisconnectVertices : public GraphRelated{
    public:
        DisconnectVertices(size_t startID, size_t endID)
            : startID_(startID), endID_(endID)
        {
            identifier_ = ID::DISCONNECT_VERTICES;
        }
        
        void execute() override{ color_ = disconnectVertices(startID_, endID_);};

        void undo() override{ connectVertices(startID_, endID_, color_);};
        void redo() override{ disconnectVertices(startID_, endID_);};

    private:
        size_t startID_;
        size_t endID_;
        
        std::optional<Color> color_;
    };

} // namespace Action