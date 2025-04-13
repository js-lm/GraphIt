#pragma once

#include "algorithms/algorithm.hpp"

#include <utility>
#include <unordered_set>
#include <queue>
#include <optional>

struct Color;

namespace Algorithm{

    class BFS{
    public:
        BFS(std::optional<VertexID> startVertex);
        ~BFS() = default;

        void run();

    private:
        std::vector<AdjacentVertex> retrieveAdjacentVertices(VertexID id);
        
        void visit(VertexID id);
        bool hasVisited(VertexID id);

        void switchCurrentVertex();

    private:
        std::optional<VertexID> startVertex_; // well, start vertex
        VertexID previousVertex_; // previous pivot vertex
        VertexID pivotVertex_; // or more like the top vertex
        // current vertex/edge means the one that is being highlighted/visited.

        std::vector<VertexID> visitedVertices_;
        std::vector<EdgeID> visitedEdge_;

        std::queue<VertexID> bfsQueue_;
    };

} // namespace Algorithm