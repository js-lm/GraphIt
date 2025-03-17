#include "graph_action.h"
#include "../../application.h"
#include "../../graph/graph.h"

#include <raylib.h>
#include <iostream>

using namespace Action;

GraphRelated::VertexID GraphRelated::addVertex(Vector2 position, std::optional<Color> color){
    return Application::instance().graph().addVertex(position, color);
}

bool GraphRelated::removeVertex(VertexID id){
    return Application::instance().graph().removeVertex(id);
}

void GraphRelated::restoreRemovedVertex(VertexID id){
    Application::instance().graph().restoreRemovedVertex(id);
}

bool GraphRelated::connectVertices(VertexID startID, VertexID endID, std::optional<Color> color){
    return Application::instance().graph().connectVertices(startID, endID, color);
}
    
std::optional<Color> GraphRelated::disconnectVertices(VertexID startID, VertexID endID){
    return Application::instance().graph().disconnectVertices(startID, endID);
}

void GraphRelated::moveVertex(VertexID id, Vector2 to){
    Application::instance().graph().updateVertexPosition(id, to);
}

void GraphRelated::bulkRemoveVertices(const std::vector<VertexID> &vertices){
    for(const auto &vertex : vertices){
        removeVertex(vertex);
    }
}

void GraphRelated::bulkRestoreRemovedVertices(const std::vector<VertexID> &vertices){
    for(const auto &vertex : vertices){
        restoreRemovedVertex(vertex);
    }
}

std::vector<std::optional<Color>> GraphRelated::bulkRemoveEdges(
    const std::vector<EdgeID> &edges
){
    std::vector<std::optional<Color>> colors;
    for(const auto &edge : edges){
        // auto color{disconnectVertices(edge.first, edge.second)};
        colors.emplace_back(disconnectVertices(edge.first, edge.second));
    }
    return colors;
}

void GraphRelated::bulkRestoreRemovedEdges(
    const std::vector<EdgeID> &edges, 
    const std::vector<std::optional<Color>> &colors
){
    if(edges.size() != colors.size()) throw;

    for(size_t i{0}; i < edges.size(); i++){
        connectVertices(edges[i].first, edges[i].second, colors[i].value());
    }
}