#pragma once

#include "../graph_action/graph_action.hpp"
#include "system/settings.hpp"

namespace Algorithm{

    class VisitVertex : public Action::GraphRelated{
    public:
        VisitVertex(
            const VertexID &vertexID,
            const EdgeID &edgeID,

            const VertexID &pivotVertex,
            const std::vector<VertexID> &visitedVertices,
            const std::vector<EdgeID> &visitedEdges
        )
            : vertexID_(vertexID)
            , edgeID_(edgeID)

            // I know this looks crazy
            // but hear me out
            // I need a way to highlight the path that is currently being visited,
            // but I don't want that color to persist
            // I could create another action (which I did)
            // but that would add an extra step, which I don't want
            // so the easiest solution is to simply recolor all the visited edges
            // same for switch_current_vertex.h
            , pivotVertex_(pivotVertex)
            , visitedVertices_(visitedVertices)
            , visitedEdges_(visitedEdges)
        {
            identifier_ = ID::VISIT_VERTEX;
        }

        void execute() override{ 
            // delete old edge highlight
            dyeSelectedVertices(visitedVertices_, appSettings.colorVertexVisited);
            dyeSelectedVertices({pivotVertex_}, appSettings.colorVertexPivot);
            dyeSelectedEdge(visitedEdges_, appSettings.colorEdgeVisited);

            dyeSelectedVertices({vertexID_}, appSettings.colorVertexCurrent);
            dyeSelectedEdge({edgeID_}, appSettings.colorEdgeCurrent);
        }

        void undo() override{ 
            dyeSelectedVertices({vertexID_}, appSettings.colorVertexUnvisited);
            dyeSelectedEdge({edgeID_}, appSettings.colorEdgeUnvisited);
        }

        void redo() override{ execute();}

    private:
        VertexID vertexID_;
        EdgeID edgeID_;

        VertexID pivotVertex_;
        std::vector<VertexID> visitedVertices_;
        std::vector<EdgeID> visitedEdges_;
    };

} // namespace Algorithm