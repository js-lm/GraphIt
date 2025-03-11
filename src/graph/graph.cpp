#include "vertex_and_edge.h"
#include "graph.h"

#include <raylib.h>
#include <assert.h>

Graph::Graph()
    : isDirected_(false)
    , defaultVertexColor_(BLACK)
    , defaultEdgeColor_(GREY)
    , vertexRadius_(5.0f)
    , edgeThickness_(10.0f)
{}

void Graph::hideVertex(size_t id){
    if(!isValidID(id)) return;
    vertices_[id]->hide();
    hiddenVertices_.insert(id);
}

void Graph::hideVertex(size_t id){
    if(!isValidID(id)) return;
    vertices_[id]->show();
    hiddenVertices_.erase(id);
}

size_t Graph::addVertex(Vector2 position, std::optional<Color> color){
    size_t id{vertices_.size()};
    
    vertices_.emplace_back(
        std::make_unique<Vertex>(
            position,
            (color ? color.value() : defaultVertexColor_),
            id
        )
    );

    return id;
}

bool Graph::removeVertex(size_t id){
    if(!isValidID(id)) return false;
    vertices[id]->hide();

    return true;
}

bool Graph::connectVertices(size_t startID, size_t endID, std::optional<Color> color){
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
            (color ? color.value() : defaultVertexColor_)
        )
    )

    return true;
}

bool Graph::disconnectVertices(size_t startID, size_t endID){
    if(!areNeighbors(startID, endID)) return false;

    std::vector<std::unique_ptr<Edge>> newEdges_;
    newEdges_.reserve(edges_.size());

    // TODO: too many duplications for edge matching, make a helper function later
    for(auto &edge : edges_){
        if((edge->startID() == startID
         && edge->endID() == endID)
        || (!isDirected_
         && edge->startID() == endID
         && edge->endID() == startID
        )) continue;

        newEdges_.emplace_back(std::move(edge));
    }

    bool isChanged{newEdges_.size() < edges_.size()};

    edges_ = std::move(newEdges_);

    return isChanged;
}

bool Graph::areNeighbors(size_t startID, size_t endID){
    if(startID == endID
    || isVertexHidden(startID)
    || isVertexHidden(endID)
    ) return false;

    for(const auto &edge : edges_){
        if(edge->startID() == startID && edge->endID() == endID){
            return true;
        }
    }

    if(!isDirected){
        for(const auto &edge : edges_){
            if(edge->startID() == endID && edge->endID() == startID){
                return true;
            }
        }
    }

    return false;
}

std::unordered_set<size_t> Graph::areNeighbors(size_t id){
    std::unordered_set<size_t> neighbors;

    if(isVertexHidden(id)) return neighbors;

    for(const auto &edge : edges_){
        if(edge->startID() == id && !isVertexHidden(edge->endID())){
            neighbors.insert(edge->endID());
        }
    }

    if(!isDirected){
        for(const auto &edge : edges_){
            if(edge->endID() == id && !isVertexHidden(edge->startID())){
                neighbors.insert(edge->startID());
            }
        }
    }

    return neighbors;
}

std::optional<size_t> Graph::checkCollisionVertex(Vector2 point, float radius){
    for(const auto &vertex : vertices_){
        if(CheckCollisionPointCircle(point, vertex->position(), radius)){
            return vertex->id();
        }
    }

    return std::nullopt
}

std::optional<std::pair<size_t, size_t>> Graph::checkCollisionEdge(Vector2 point, float thickness){
    for(const auto &edge : edges_){
        if(!isVertexHidden(edge->startID())
        && !isVertexHidden(edge->endID())
        && CheckCollisionPointLine(
                point, 
                vertices_[edge->startID()]->position(), 
                vertices_[edge->endID()]->position(),
                thickness
            )
        ) return {edge->startID(), edge->endID()};
    }

    return std::nullopt;
}