#pragma once

#include "../action_base_class.hpp"

namespace Action{

    class GraphRelated : public ActionBaseClass{
    protected:
        using VertexID = size_t;
        using EdgeID = std::pair<VertexID, VertexID>;

    public:
        GraphRelated(){ shouldBeRecorded_ = true;}
        virtual ~GraphRelated() = default;

        virtual void execute() override = 0;

        virtual void undo() override = 0;
        virtual void redo() override = 0;

    protected:
        VertexID addVertex(Vector2 position, Color color);
        bool removeVertex(VertexID id);
        void restoreRemovedVertex(VertexID id);

        bool connectVertices(VertexID startID, VertexID endID, Color color, float weight);
        std::pair<float, Color> disconnectVertices(VertexID startID, VertexID endID);

        void moveVertex(VertexID id, Vector2 to);

        void bulkRemoveVertices(const std::vector<VertexID> &vertices);
        void bulkRestoreRemovedVertices(const std::vector<VertexID> &vertices);
        std::vector<std::pair<float,Color>> bulkRemoveEdges(const std::vector<EdgeID> &edges);
        void bulkRestoreRemovedEdges(const std::vector<EdgeID> &edges, const std::vector<float> &weight, const std::vector<Color> &colors);

        std::vector<Color> dyeSelectedVertices(const std::vector<VertexID> &ids, Color newColor);
        std::vector<Color> dyeSelectedEdge(const std::vector<EdgeID> &ids, Color newColor);
        
        void dyeSelectedVertices(const std::vector<VertexID> &ids, const std::vector<Color> &newColor);
        void dyeSelectedEdge(const std::vector<EdgeID> &ids, const std::vector<Color> &newColor);

        std::vector<float> reweighSelectedEdge(const std::vector<EdgeID> &ids, float newWeight);
        void reweighSelectedEdge(const std::vector<EdgeID> &ids, const std::vector<float> &newWeight);
    };

} // namespace Action