#pragma once

#include "../graph_action/graph_action.hpp"
#include "system/settings.hpp"

namespace Algorithm{

    class SwitchCurrentVertex : public Action::GraphRelated{
    public:
        SwitchCurrentVertex(
            const VertexID &currentID, 
            const VertexID &previousID,

            const std::vector<VertexID> &visitedVertices,
            const std::vector<EdgeID> &visitedEdges
        )
            : currentID_(currentID)
            , previousID_(previousID)

            // hi, future me
            // if you are wondering what the hell this is
            // go to visit_vertex.h
            , visitedVertices_(visitedVertices)
            , visitedEdges_(visitedEdges) 
        {
            identifier_ = ID::SWITCH_CURRENT_VERTEX;
        }

        void execute() override{ 
            // delete old edge highlight
            dyeSelectedVertices(visitedVertices_, appSettings.colorVertexVisited);
            dyeSelectedEdge(visitedEdges_, appSettings.colorEdgeVisited);

            dyeSelectedVertices({previousID_}, appSettings.colorVertexVisited);
            dyeSelectedVertices({currentID_}, appSettings.colorVertexPivot);
        }

        void undo() override{ 
            dyeSelectedVertices({previousID_}, appSettings.colorVertexPivot);
            dyeSelectedVertices({currentID_}, appSettings.colorVertexVisited);
        }

        void redo() override{ execute();}

    private:
        VertexID currentID_;
        VertexID previousID_;

        std::vector<VertexID> visitedVertices_;
        std::vector<EdgeID> visitedEdges_;
    };

} // namespace Algorithm