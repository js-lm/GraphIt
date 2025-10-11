#pragma once

#include "algorithms/algorithm.hpp"

#include <utility>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <optional>
#include <limits>

struct Color;

namespace Algorithm{

    class Dijkstra{
    public:
        Dijkstra(std::optional<VertexID> startVertex);
        ~Dijkstra() = default;

        void run();

    private:
        std::vector<AdjacentVertex> retrieveAdjacentVertices(VertexID vertexID);
        
        void visit(VertexID vertexID, float distance);
        bool hasVisited(VertexID vertexID);

    private:
        struct VertexDistance{
            VertexID vertexID;
            float distance;
            
            bool operator>(const VertexDistance &other) const{
                return distance > other.distance;
            }
        };

        std::optional<VertexID> startVertex_;
        VertexID previousVertex_;
        VertexID pivotVertex_;

        std::vector<VertexID> visitedVertices_;
        std::vector<EdgeID> visitedEdges_;

        std::unordered_map<VertexID, float> distances_;
        std::unordered_map<VertexID, VertexID> predecessors_;
        
        std::priority_queue<
            VertexDistance, 
            std::vector<VertexDistance>, 
            std::greater<VertexDistance>
        > priorityQueue_;
    };

} // namespace Algorithm
