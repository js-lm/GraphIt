#include "graph.h"
#include "../application.h"

#include <raylib.h>
#include <iostream>

Graph::Graph()
    : isDirected_(false)
    , defaultVertexColor_(BLACK)
    , defaultEdgeColor_(GRAY)
    , vertexRadius_(5.0f)
    , edgeThickness_(10.0f)  
{}

void Graph::draw() const{
    drawEdges();
    drawVertices();
}

void Graph::drawVertices() const{
    // auto &hoveredVertexID{Application::instance().graph()->getHoveredVertexID()};
    // auto &selectedVertexIDs{Application::instance().graph()->getSelectedVertexIDs()};

    for(const auto &vertex : vertices_){
        if(vertex->isHidden()) continue;
        
        float vertexRadius{vertexRadius_};

        // if(hoveredVertexID == vertex->id()) vertexRadius *= 1.25f;

        DrawCircleV(vertex->position(), vertexRadius, vertex->color());

        // if(selectedVertexIDs.find(vertex->id()) != selectedVertexIDs.end()){ 
        //     DrawCircleLinesV(vertex->position(), vertexRadius * 1.25f, vertex->color());
        // } 
    }
}

void Graph::drawEdges() const{
    // auto &hoveredEdgeIDs{Application::instance().graph()->getHoveredEdgeIDs()};
    // auto &selectedEdgeIDs{Application::instance().graph()->getSelectedEdgeIDs()};

    for(const auto &edge : edges_){
        if(isVertexHidden(edge->startID())
        || isVertexHidden(edge->endID())
        ){
            continue;
        }
        
        float edgeThickness{edgeThickness_};

        // if(hoveredEdgeIDs == edge->id()) edgeThickness *= 1.25f;

        DrawLineEx(
            vertices_[edge->startID()]->position(), 
            vertices_[edge->endID()]->position(), 
            edgeThickness, 
            edge->color()
        );
        
        // for(const auto &[startID, endID] : selectedEdgeIDs){
        //     if(isTheSameEdge(edge->startID(), edge->endID(), startID, endID)){
        //         DrawLineEx(
        //             vertices_[edge->startID()]->position(), 
        //             vertices_[edge->endID()]->position(), 
        //             edgeThickness_ / 2, 
        //             ColorContrast(edge->color(), .5f)
        //         );
        //     }
        // }
    }
}

void Graph::hideVertex(VertexID id){
    if(!isValidID(id)) return;
    vertices_[id]->hide();
    hiddenVertices_.insert(id);
}

void Graph::showVertex(VertexID id){
    if(!isValidID(id)) return;
    vertices_[id]->show();
    hiddenVertices_.erase(id);
}

bool Graph::isVertexHidden(VertexID id) const{
    if(!isValidID(id)) return true;
    return vertices_[id]->isHidden();
}

Graph::VertexID Graph::addVertex(Vector2 position, std::optional<Color> color){
    VertexID id{vertices_.size()};

    std::cout << "Graph::addVertex() called\n";
    
    vertices_.emplace_back(
        std::make_unique<Vertex>(
            position,
            color.value_or(defaultVertexColor_),
            id
        )
    );

    return id;
}

bool Graph::removeVertex(VertexID id){
    if(!isValidID(id)) return false;
    hideVertex(id);

    return true;
}

bool Graph::restoreRemovedVertex(VertexID id){
    if(!isVertexHidden(id)) return false;
    showVertex(id);
    return !isVertexHidden(id);
}

bool Graph::connectVertices(VertexID startID, VertexID endID, std::optional<Color> color){
    if(!isValidID(startID)
    || !isValidID(endID)
    || startID == endID
    || isVertexHidden(startID)
    || isVertexHidden(endID)
    || areNeighbors(startID, endID)
    ) return false;

    edges_.emplace_back(
        std::make_unique<Edge>(
            startID,
            endID,
            color.value_or(defaultEdgeColor_)
        )
    );

    return true;
}

std::optional<Color> Graph::disconnectVertices(VertexID startID, VertexID endID){
    std::optional<Color> color{std::nullopt};

    if(!areNeighbors(startID, endID)) return color;

    std::vector<std::unique_ptr<Edge>> newEdges_;
    newEdges_.reserve(edges_.size());

    for(auto &edge : edges_){
        if(isTheSameEdge(edge->startID(), edge->endID(), startID, endID)){
            color = edge->color();
            continue;
        }

        newEdges_.emplace_back(std::move(edge));
    }
    
    edges_ = std::move(newEdges_);

    return color;
}

bool Graph::areNeighbors(VertexID startID, VertexID endID){
    if(startID == endID
    || isVertexHidden(startID)
    || isVertexHidden(endID)
    ){
        return false;
    }

    for(const auto &edge : edges_){
        if(isTheSameEdge(edge->startID(), edge->endID(), startID, endID)){
            return true;
        }
    }

    return false;
}

std::unordered_set<Graph::VertexID> Graph::getNeighbors(VertexID id) const{
    std::unordered_set<VertexID> neighbors;

    if(isVertexHidden(id)) return neighbors;

    for(const auto &edge : edges_){
        if(edge->startID() == id && !isVertexHidden(edge->endID())){
            neighbors.insert(edge->endID());
        }
    }

    if(!isDirected_){
        for(const auto &edge : edges_){
            if(edge->endID() == id && !isVertexHidden(edge->startID())){
                neighbors.insert(edge->startID());
            }
        }
    }

    return neighbors;
}

std::optional<Graph::VertexID> Graph::findVertex(Vector2 point, float radius){
    for(const auto &vertex : vertices_){
        if(!vertex->isHidden() && CheckCollisionPointCircle(point, vertex->position(), radius)){
            return vertex->id();
        }
    }

    return std::nullopt;
}

std::optional<Graph::EdgeID> Graph::findEdge(Vector2 point, float thickness){
    for(const auto &edge : edges_){
        if(!isVertexHidden(edge->startID())
        && !isVertexHidden(edge->endID())
        && CheckCollisionPointLine(
                point, 
                vertices_[edge->startID()]->position(), 
                vertices_[edge->endID()]->position(),
                thickness
            )
        ){
            return std::make_pair(edge->startID(), edge->endID());
        }
    }

    return std::nullopt;
}

bool Graph::isTheSameEdge(VertexID start1, VertexID end1, VertexID start2, VertexID end2){
    return ((start1 == start2
            && end1 == end2)
           || (!isDirected_
            && start1 == end2
            && end1 == start2
           ));
}