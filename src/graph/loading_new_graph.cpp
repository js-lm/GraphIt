#include "graph.h"
#include "application.h"
#include "actions_center/actions_center.h"

std::vector<Graph::VertexID> Graph::getAllValidVertexIDs() const{
    std::vector<VertexID> ids;
    for(const auto &vertex : vertices_){
        if(!vertex->isHidden()){
            ids.emplace_back(vertex->id());
        }
    }
    return ids;
}

std::vector<std::pair<Graph::EdgeID, Color>> Graph::getAllValidEdgeIDsAndColor() const{
    std::vector<std::pair<Graph::EdgeID, Color>> ids;
    for(const auto &edge : edges_){
        if(!vertices_[edge->startID()]->isHidden()
        && !vertices_[edge->endID()]->isHidden()
        ){
            ids.emplace_back(std::pair{
                EdgeID{edge->startID(), edge->endID()},
                edge->color()        
            });
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