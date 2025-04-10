#pragma once

#include "io/io.hpp"

#include <unordered_map>
#include <optional>

struct Color;

namespace Algorithm{

    class Kruskal{
    public:
        using VertexID = size_t;
        using EdgeID = std::pair<VertexID, VertexID>;
                
    public:
        Kruskal();
        ~Kruskal() = default;

    private:
        void initializeGraphAndMaps();

        VertexID getParent(VertexID id);
        void combine(VertexID id1, VertexID id2);

    private:
        std::vector<Normalized::Edge> graph_;

        // disjointed sets
        std::unordered_map<VertexID, VertexID> parentsTable_;
        std::unordered_map<VertexID, int> heightsTable_;

        EdgeID previousEdge_;
        bool isPreviousRejected_;
    };

} // namespace Algorithm