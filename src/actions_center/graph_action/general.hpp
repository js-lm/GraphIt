#pragma once

#include "graph_action.hpp"

namespace Action{

class AddVertex : public GraphRelated{
public:
    AddVertex(Vector2 position, Color color)
        : position_(position), color_(color){ identifier_ = ID::ADD_VERTEX;}
    ~AddVertex() = default;

    void execute() override{ vertexId_ = addVertex(position_, color_);}

    void undo() override{ removeVertex(vertexId_);}
    void redo() override{ restoreRemovedVertex(vertexId_);}

private:
    Vector2 position_;
    Color color_;
    VertexID vertexId_;
};

class RemoveVertex : public GraphRelated{
public:
    RemoveVertex(VertexID id) : vertexID_(id){ identifier_ = ID::REMOVE_VERTEX;}
    ~RemoveVertex() = default;

    void execute() override{ removeVertex(vertexID_);}

    void undo() override{ restoreRemovedVertex(vertexID_);}
    void redo() override{ execute();}

private:
    VertexID vertexID_;
};

class ConnectVertices : public GraphRelated{
public:
    ConnectVertices(VertexID startID, VertexID endID, float weight, Color color)
        : startID_(startID), endID_(endID)
        , weight_(weight)
        , color_(color)
    {
        identifier_ = ID::CONNECT_VERTICES;
    };
    
    void execute() override{ connectVertices(startID_, endID_, color_, weight_);}

    void undo() override{ disconnectVertices(startID_, endID_);}
    void redo() override{ execute();}

private:
    VertexID startID_;
    VertexID endID_;
    float weight_;
    Color color_;
};

class DisconnectVertices : public GraphRelated{
public:
    DisconnectVertices(VertexID startID, VertexID endID)
        : startID_(startID), endID_(endID)
    {
        identifier_ = ID::DISCONNECT_VERTICES;
    }
    
    void execute() override{ 
        auto properties{disconnectVertices(startID_, endID_)};
        weight_ = properties.first;
        color_ = properties.second;
    };

    void undo() override{ connectVertices(startID_, endID_, color_, weight_);}
    void redo() override{ disconnectVertices(startID_, endID_);}

private:
    VertexID startID_;
    VertexID endID_;
    float weight_;
    Color color_;
};

class MoveVertex : public GraphRelated{
public:
    MoveVertex(VertexID id, Vector2 from, Vector2 to)
        : id_(id), from_(from), to_(to)
    {
        identifier_ = ID::MOVE_VERTEX;
    }
    
    void execute() override{};

    void undo() override{ moveVertex(id_, from_);}
    void redo() override{ moveVertex(id_, to_);}

private:
    VertexID id_;
    
    Vector2 from_;
    Vector2 to_;
};

} // namespace Action