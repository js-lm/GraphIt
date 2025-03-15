#pragma once

#include "vertex_and_edge.h"

#include <raylib.h>
#include <memory>
#include <vector>
#include <optional>
#include <utility>
#include <unordered_set>

class Graph{
public:
    using Vertex = GraphNamespace::Vertex;
    using Edge = GraphNamespace::Edge;

    using VertexID = size_t;
    using EdgeID = std::pair<VertexID, VertexID>;

public:
    Graph();
    ~Graph() = default;

    void draw() const;

public:
VertexID addVertex(Vector2 position, std::optional<Color> color = std::nullopt);
    bool removeVertex(VertexID id);
    bool restoreRemovedVertex(VertexID id);

    bool connectVertices(VertexID startID, VertexID endID, std::optional<Color> color = std::nullopt);
    std::optional<Color> disconnectVertices(VertexID startID, VertexID endID);

    bool areNeighbors(VertexID startID, VertexID endID);
    std::unordered_set<VertexID> getNeighbors(VertexID id) const;

public:
    std::optional<VertexID> findVertex(Vector2 point, float radius);
    std::optional<EdgeID> findEdge(Vector2 point, float thickness);

    const float getVertexRadius() const{ return vertexRadius_;};
    const float getEdgeThickness() const{ return edgeThickness_;};

private:
    void drawVertices() const;
    void drawEdges() const;

    bool isValidID(VertexID id) const{ return id < vertices_.size();};
    bool isVertexHidden(size_t id) const;

    void hideVertex(VertexID id);
    void showVertex(VertexID id);

    bool isTheSameEdge(VertexID startID1, VertexID endID1, VertexID startID2, VertexID endID2);

private:
    bool isDirected_;

    std::vector<std::unique_ptr<Vertex>> vertices_;
    std::vector<std::unique_ptr<Edge>> edges_;
    // std::unordered_map<size_t> adjacencyMap_;
    // std::vector<size_t, std::vector<size_t, bool>> adjacencyMatrix_;
    std::unordered_set<VertexID> hiddenVertices_;

    Color defaultVertexColor_;
    Color defaultEdgeColor_;

    float vertexRadius_;
    float edgeThickness_;
};