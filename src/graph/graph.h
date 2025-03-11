#pragma once

#include <memory>
#include <vector>
#include <optional>
#include <utility>
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
    Graph();
    ~Graph() = default;

    // void draw() const;

    size_t addVertex(Vector2 position, std::optional<Color> color = std::nullopt);
    bool removeVertex(size_t id);

    bool connectVertices(size_t startID, size_t endID, std::optional<Color> color = std::nullopt);
    bool disconnectVertices(size_t startID, size_t endID);

    bool areNeighbors(size_t startID, size_t endID) const;
    std::unordered_set<size_t> getNeighbors(size_t startID) const;

    std::optional<size_t> checkCollisionVertex(Vector2 point, float radius);
    std::optional<std::pair<size_t, size_t>> checkCollisionEdge(Vector point, float thickness);

private:
    // void drawVertices() const;
    // void drawEdges() const;

    bool isValidID(size_t id) const{ return id < vertices_.size();};
    bool isVertexHidden(size_t id) const{ return vertices[id]->isHidden();};

    void hideVertex(size_t id);
    void showVertex(size_t id);

private:
    bool isDirected_;

    // pop = delete
    std::vector<std::unique_ptr<Vertex>> vertices_;
    std::vector<std::unique_ptr<Edge>> edges_;
    // std::unordered_map<size_t> adjacencyMap_;
    // std::vector<size_t, std::vector<size_t, bool>> adjacencyMatrix_;
    std::unordered_set<size_t> hiddenVertices_;

    Color defaultVertexColor_;
    Color defaultEdgeColor_;
    float vertexRadius_;
    float edgeThickness_;
};