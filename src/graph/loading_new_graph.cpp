#include "graph.hpp"
#include "system/application.hpp"
#include "actions_center/actions_center.hpp"

std::vector<Graph::VertexID> Graph::getAllValidVertexIDs() const{
    std::vector<VertexID> ids;
    for(const auto &vertex : vertices_){
        if(!vertex->isHidden()){
            ids.emplace_back(vertex->id());
        }
    }
    return ids;
}

std::vector<Normalized::Edge> Graph::getAllValidEdges() const{
    std::vector<Normalized::Edge> ids;
    for(const auto &edge : edges_){
        if(!vertices_[edge->startID()]->isHidden()
        && !vertices_[edge->endID()]->isHidden()
        ){
            Normalized::Edge normalizedEdge;

            normalizedEdge.startID = edge->startID();
            normalizedEdge.endID = edge->endID();
            normalizedEdge.weight = edge->weight();
            normalizedEdge.color = edge->color();

            ids.emplace_back(normalizedEdge);
        }
    }
    return ids;
}

void Graph::clearGraph(){
    Application::instance().actionCenter().clearHistory();
    vertices_.clear();
    edges_.clear();
    hiddenVertices_.clear();
}

void Graph::loadNewGraph(const Normalized::SaveData &saveData){
    clearGraph();

    auto &vertices{saveData.vertices};
    auto &edges{saveData.edges};

    for(const auto &vertex : vertices){
        addVertex(vertex.id, vertex.position, vertex.color);
    }

    for(const auto &edge : edges){
        connectVertices(edge.startID, edge.endID, edge.weight, edge.color);
    }
}

void Graph::addVertex(size_t id, Vector2 position, Color color){
    vertices_.emplace_back(
        std::make_unique<Vertex>(
            position,
            color,
            id
        )
    );
}

void Graph::connectVertices(VertexID startID, VertexID endID, float weight, Color color){
    if(!(isValidID(startID) 
      && isValidID(endID) 
      && startID != endID
      && !areNeighbors(startID, endID)
    )) return;
    
    edges_.emplace_back(
        std::make_unique<Edge>(
            startID,
            endID,
            weight,
            color
        )
    );
}