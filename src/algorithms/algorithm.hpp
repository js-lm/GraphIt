#pragma once

#include <cstddef>
#include <vector>
#include <utility>

namespace Algorithm{

    using VertexID = size_t;
    using EdgeID = std::pair<VertexID, VertexID>;

    struct AdjacentVertex{
        VertexID id;
        float weight;
    };

    struct Vertex{
        VertexID id;
        std::vector<AdjacentVertex> adjacentVertices;
    };

} // namespace Algorithm