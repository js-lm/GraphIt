#pragma once

#include "../graph_action/graph_action.hpp"
#include "system/settings.hpp"

#include <utility>
#include <optional>

namespace Action{

    class KruskalAddEdge : public GraphRelated{
    public:
        KruskalAddEdge(
            EdgeID id, 
            EdgeID previousEdge, 
            bool isCurrentEdgeRejected, 
            bool isPreviousEdgeRejected
        )
            : id_(id)
            , previousEdge_(previousEdge)
            , isCurrentEdgeRejected_(isCurrentEdgeRejected)
            , isPreviousEdgeRejected_(isPreviousEdgeRejected)
        {
            identifier_ = ID::KRUSKAL;
        }

        void execute() override{ 
            originalVertexColors_ = dyeSelectedVertices({id_.first, id_.second}, appSettings.colorVertexVisited);
            originalEdgeColors_ = dyeSelectedEdge({id_}, appSettings.colorEdgeCurrent);
        }

        void undo() override{ 
            dyeSelectedVertices({id_.first, id_.second}, originalVertexColors_);
            dyeSelectedEdge({id_}, originalEdgeColors_);
        }

        void redo() override{ 
            dyeSelectedVertices({id_.first, id_.second}, appSettings.colorVertexVisited);
            dyeSelectedEdge(
                {id_}, 
                isCurrentEdgeRejected_ ? appSettings.colorEdgeBlocked : appSettings.colorEdgeCurrent
            );
            dyeSelectedEdge(
                {previousEdge_}, 
                isPreviousEdgeRejected_ ? appSettings.colorEdgeUnvisited : appSettings.colorEdgeVisited
            );
        }

    private:
        EdgeID id_;

        std::vector<Color> originalVertexColors_;
        std::vector<Color> originalEdgeColors_;

        EdgeID previousEdge_;
        bool isCurrentEdgeRejected_;
        bool isPreviousEdgeRejected_;
    };

} // namespace Action