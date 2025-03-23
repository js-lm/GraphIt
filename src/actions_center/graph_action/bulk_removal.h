#pragma once

#include "graph_action.h"

namespace Action{

    class BulkRemove : public GraphRelated{
    public:
        BulkRemove(const std::vector<VertexID> &vertexIDs, const std::vector<EdgeID> &edgeIDs)
            : vertexIDs_(vertexIDs), edgeIDs_(edgeIDs)
        {
            identifier_ = ID::BULK_REMOVE;
        }

        void execute() override{ 
            auto edgesPropertiesVector{bulkRemoveEdges(edgeIDs_)};
            for(const auto &properties : edgesPropertiesVector){
                edgeWeights_.emplace_back(properties.first);
                edgeColors_.emplace_back(properties.second);
            }
            bulkRemoveVertices(vertexIDs_);
        }

        void undo() override{ 
            bulkRestoreRemovedVertices(vertexIDs_);
            bulkRestoreRemovedEdges(edgeIDs_, edgeWeights_, edgeColors_);
        }
        
        void redo() override{ 
            bulkRemoveEdges(edgeIDs_);
            bulkRemoveVertices(vertexIDs_);
        }

    private:
        std::vector<VertexID> vertexIDs_;
        std::vector<EdgeID> edgeIDs_;
        std::vector<float> edgeWeights_;
        std::vector<Color> edgeColors_;
    };

    class BulkRemoveVertices : public GraphRelated{
    public:
        BulkRemoveVertices(const std::vector<VertexID> &ids)
            : ids_(ids)
        {
            identifier_ = ID::BULK_REMOVE_VERTICES;
        }

        void execute() override{ bulkRemoveVertices(ids_);}

        void undo() override{ bulkRestoreRemovedVertices(ids_);}
        void redo() override{ execute();}

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

        void execute() override{
            auto propertiesVector{bulkRemoveEdges(ids_)};
            for(const auto &properties : propertiesVector){
                weight_.emplace_back(properties.first);
                colors_.emplace_back(properties.second);
            }
        };

        void undo() override{ bulkRestoreRemovedEdges(ids_, weight_, colors_);}
        void redo() override{ bulkRemoveEdges(ids_);}

    private:
        std::vector<EdgeID> ids_;
        std::vector<float> weight_;
        std::vector<Color> colors_;
    };

} // namespace Action