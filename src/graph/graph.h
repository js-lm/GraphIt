#pragma once

#include <memory>
#include <vector>
#include <optional>
#include <pairs>
#include <unordered_set>

namespace GraphNamespace{
    class Vertex;
    class Edge;
}

struct Vector2;
struct Color;

class Graph{
public:
    using Vertex = GraphNamespace::Vertex;
    using Edge = GraphNamespace::Edge;

public:
    static std::unique_ptr<Graph> createGraph();

public:
    size_t addVertex(Vector2 position, std::optional<Color> color = std::nullopt);
    bool removeVertex(size_t id);

    bool connectVertices(size_t startID, size_t endID);
    bool disconnectVertices(size_t startID, size_t endID);

    std::optional<size_t> checkCollisionVertexPoint(Vector2 point);
    std::optional<std::pair<size_t, size_t>> checkCollisionEdgePoint(Vector point);

private:
    Graph();
    ~Graph() = default;

    bool isDirected;

    // pop = delete
    std::vector<std::unique_ptr<Vertex>> vertices_;
    std::vector<std::unique_ptr<Edge>> edges_;
    std::unordered_set<size_t> hiddenVertices_;

    Color defaultVertexColor_;
    Color defaultEdgeColor_;
    float defaultVertexRadius_;
    float defaultEdgeThickness_;
};