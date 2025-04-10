#pragma once

#include "vertex_and_edge.hpp"
#include "io/io.hpp"

#include <raylib.h>
#include <memory>
#include <vector>
#include <optional>
#include <utility>
#include <unordered_set>
#include <string>

class Graph{
public:
    using Vertex = GraphNamespace::Vertex;
    using Edge = GraphNamespace::Edge;

    using VertexID = size_t;
    using EdgeID = std::pair<VertexID, VertexID>;

public:
    Graph() = default;
    ~Graph() = default;

    void draw() const;

public:
    VertexID addVertex(Vector2 position, Color color);
    bool removeVertex(VertexID id);
    bool restoreRemovedVertex(VertexID id);

    bool connectVertices(VertexID startID, VertexID endID, Color color, float weight);
    std::pair<float, Color> disconnectVertices(VertexID startID, VertexID endID);

    std::vector<Color> dyeSelectedVertices(const std::vector<VertexID> &ids, Color newColor);
    std::vector<Color> dyeSelectedEdge(const std::vector<EdgeID> &ids, Color newColor);

    void dyeSelectedVertices(const std::vector<VertexID> &ids, const std::vector<Color> &newColor);
    void dyeSelectedEdge(const std::vector<EdgeID> &ids, const std::vector<Color> &newColor);

    std::vector<float> reweighSelectedEdge(const std::vector<EdgeID> &ids, float newWeight);
    void reweighSelectedEdge(const std::vector<EdgeID> &ids, const std::vector<float> &newWeights);

    void updateVertexPosition(VertexID id, Vector2 position){ vertices_[id]->setPosition(position);}
    Vector2 getVertexPosition(VertexID id){ return vertices_[id]->position();}
    Color getVertexColor(VertexID id){ return vertices_[id]->color();}

    bool areNeighbors(VertexID startID, VertexID endID);
    std::vector<std::pair<VertexID, float>> retrieveAdjacentVertices(VertexID id) const;

    std::string getLabel(VertexID id) const;

public:
    std::optional<VertexID> findVertex(Vector2 point, std::optional<float> radius = std::nullopt);
    std::optional<EdgeID> findEdge(Vector2 point, std::optional<float> thickness = std::nullopt);

    std::vector<VertexID> findVertex(Rectangle area);
    std::vector<EdgeID> findEdge(Rectangle area);

public: // for IO
    std::vector<VertexID> getAllNonHiddenVertexIDs() const;
    std::vector<Normalized::Edge> getAllNonHiddenEdges() const;

    void loadNewGraph(const Normalized::SaveData &saveData);
    void clearGraph();

private:
    void addVertex(size_t id, Vector2 position, Color color);
    void connectVertices(VertexID startID, VertexID endID, float weight, Color color);

private:
    void drawVertices() const;
    void drawEdges() const;
    void drawArrow(
        const Vector2 &startPoint, 
        const Vector2 &endPoint, 
        float thickness,  
        Color color
    ) const;
    void drawLabels() const;
    void drawWeights() const;

    bool isValidID(VertexID id) const{ return id < vertices_.size();}
    bool isVertexHidden(size_t id) const;

    void hideVertex(VertexID id);
    void showVertex(VertexID id);

    bool isTheSameEdge(VertexID startID1, VertexID endID1, VertexID startID2, VertexID endID2) const;

    bool checkCollisionLineRectangle(Vector2 start, Vector2 end, Rectangle rectangle) const;

    float calculateBrightness(Color color) const;

private:
    std::vector<std::unique_ptr<Vertex>> vertices_;
    std::vector<std::unique_ptr<Edge>> edges_;
    // std::unordered_map<size_t> adjacencyMap_;
    // std::vector<size_t, std::vector<size_t, bool>> adjacencyMatrix_;
    std::unordered_set<VertexID> hiddenVertices_;
};