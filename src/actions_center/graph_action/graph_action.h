#pragma once

#include "../action_base_class.h"

#include <optional>
#include <raylib.h>
#include <utility>

namespace Action{

    class GraphRelated : public ActionBaseClass{
    protected:
        using VertexID = size_t;
        using EdgeID = std::pair<VertexID, VertexID>;

    public:
        GraphRelated(){ shouldBeRecorded_ = true;};
        virtual ~GraphRelated() = default;

        virtual void execute() override = 0;

        virtual void undo() override = 0;
        virtual void redo() override = 0;

    protected:
        VertexID addVertex(Vector2 position, std::optional<Color> color = std::nullopt);
        bool removeVertex(VertexID id);
        void restoreRemovedVertex(VertexID id);

        bool connectVertices(VertexID startID, VertexID endID, std::optional<Color> color = std::nullopt);
        std::optional<Color> disconnectVertices(VertexID startID, VertexID endID);

        void moveVertex(VertexID id, Vector2 to);

        void bulkRemoveVertices(const std::vector<VertexID> &vertices);
        void bulkRestoreRemovedVertices(const std::vector<VertexID> &vertices);
        std::vector<std::optional<Color>> bulkRemoveEdges(const std::vector<EdgeID> &edges);
        void bulkRestoreRemovedEdges(const std::vector<EdgeID> &edges, const std::vector<std::optional<Color>> &colors);

        std::vector<Color> dyeSelectedVertices(const std::vector<VertexID> &ids, Color newColor);
        std::vector<Color> dyeSelectedEdge(const std::vector<EdgeID> &ids, Color newColor);
        
        void dyeSelectedVertices(const std::vector<VertexID> &ids, const std::vector<Color> &newColor);
        void dyeSelectedEdge(const std::vector<EdgeID> &ids, const std::vector<Color> &newColor);
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
        VertexID vertexId_;
    };

    class RemoveVertex : public GraphRelated{
    public:
        RemoveVertex(VertexID id) : vertexID_(id){ identifier_ = ID::REMOVE_VERTEX;};
        ~RemoveVertex() = default;

        void execute() override{ removeVertex(vertexID_);};

        void undo() override{ restoreRemovedVertex(vertexID_);};
        void redo() override{ execute();};

    private:
        VertexID vertexID_;
    };

    class ConnectVertices : public GraphRelated{
    public:
        ConnectVertices(VertexID startID, VertexID endID, std::optional<Color> color = std::nullopt)
            : startID_(startID), endID_(endID)
            , color_(color)
        {
            identifier_ = ID::CONNECT_VERTICES;
        };
        
        void execute() override{ connectVertices(startID_, endID_, color_);};

        void undo() override{ disconnectVertices(startID_, endID_);};
        void redo() override{ execute();};

    private:
        VertexID startID_;
        VertexID endID_;
        
        std::optional<Color> color_;
    };

    class DisconnectVertices : public GraphRelated{
    public:
        DisconnectVertices(VertexID startID, VertexID endID)
            : startID_(startID), endID_(endID)
        {
            identifier_ = ID::DISCONNECT_VERTICES;
        }
        
        void execute() override{ color_ = disconnectVertices(startID_, endID_);};

        void undo() override{ connectVertices(startID_, endID_, color_);};
        void redo() override{ disconnectVertices(startID_, endID_);};

    private:
        VertexID startID_;
        VertexID endID_;
        
        std::optional<Color> color_;
    };

    class MoveVertex : public GraphRelated{
    public:
        MoveVertex(VertexID id, Vector2 from, Vector2 to)
            : id_(id), from_(from), to_(to)
        {
            identifier_ = ID::MOVE_VERTEX;
        }
        
        void execute() override{};

        void undo() override{ moveVertex(id_, from_);};
        void redo() override{ moveVertex(id_, to_);};

    private:
        VertexID id_;
        
        Vector2 from_;
        Vector2 to_;

    };

    class BulkRemove : public GraphRelated{
    public:
        BulkRemove(const std::vector<VertexID> &vertexIDs, const std::vector<EdgeID> &edgeIDs)
            : vertexIDs_(vertexIDs), edgeIDs_(edgeIDs)
        {
            identifier_ = ID::BULK_REMOVE;
        }

        void execute() override{ 
            edgeColors_ = bulkRemoveEdges(edgeIDs_);
            bulkRemoveVertices(vertexIDs_);
        }

        void undo() override{ 
            bulkRestoreRemovedVertices(vertexIDs_);
            bulkRestoreRemovedEdges(edgeIDs_, edgeColors_);
        }
        
        void redo() override{ 
            bulkRemoveEdges(edgeIDs_);
            bulkRemoveVertices(vertexIDs_);
        }

    private:
        std::vector<VertexID> vertexIDs_;
        std::vector<EdgeID> edgeIDs_;
        std::vector<std::optional<Color>> edgeColors_;
    };

    class BulkRemoveVertices : public GraphRelated{
    public:
        BulkRemoveVertices(const std::vector<VertexID> &ids)
            : ids_(ids)
        {
            identifier_ = ID::BULK_REMOVE_VERTICES;
        }

        void execute() override{ bulkRemoveVertices(ids_);};

        void undo() override{ bulkRestoreRemovedVertices(ids_);};
        void redo() override{ execute();};

    private:
        std::vector<VertexID> ids_;
    };

    class BulkRemoveEdges : public GraphRelated{
    public:
        BulkRemoveEdges(const std::vector<EdgeID> &ids)
            : ids_(ids)
        {
            identifier_ = ID::BULK_REMOVE_EDGES;
        }

        void execute() override{ colors_ = bulkRemoveEdges(ids_);};

        void undo() override{ bulkRestoreRemovedEdges(ids_, colors_);};
        void redo() override{ bulkRemoveEdges(ids_);};

    private:
        std::vector<EdgeID> ids_;
        std::vector<std::optional<Color>> colors_;
    };

    class DyeVertex : public GraphRelated{
    public:
        DyeVertex(
            const std::vector<VertexID> &ids,
            Color newColor
        )
            : ids_(ids), newColor_(newColor)
        {}

        void execute() override{ oldColor_ = dyeSelectedVertices(ids_, newColor_);};

        void undo() override{ dyeSelectedVertices(ids_, oldColor_);};
        void redo() override{ dyeSelectedVertices(ids_, newColor_);};

    private:
        std::vector<VertexID> ids_;
        Color newColor_;
        std::vector<Color> oldColor_;
    };

    class DyeEdge : public GraphRelated{
    public:
        DyeEdge(
            const std::vector<EdgeID> &ids,
            Color newColor
        )
            : ids_(ids), newColor_(newColor)
        {}

        void execute() override{ oldColor_ = dyeSelectedEdge(ids_, newColor_);};

        void undo() override{ dyeSelectedEdge(ids_, oldColor_);};
        void redo() override{ dyeSelectedEdge(ids_, newColor_);};

    private:
        std::vector<EdgeID> ids_;
        Color newColor_;
        std::vector<Color> oldColor_;
    };

} // namespace Action