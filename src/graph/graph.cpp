#include "vertex_and_edge.h"

#include <raylib.h>

Graph::Graph()
    : isDirected(false)
    , defaultVertexColor_(BLACK)
    , defaultEdgeColor_(GREY)
    , defaultVertexRadius_(5.0f)
    , defaultEdgeThickness_(10.0f)
{}

static std::unique_ptr<Graph> Graph::createGraph(){
    static bool created{false};
    if(!created){
        created = true;
        return std::make_unique<Graph>();
    }else{
        return nullptr;
    }
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

}

bool Graph::connectVertices(size_t startID, size_t endID){

}

bool Graph::disconnectVertices(size_t startID, size_t endID){

}

std::optional<size_t> Graph::checkCollisionVertexPoint(Vector2 point){

}

std::optional<std::pair<size_t, size_t>> Graph::checkCollisionEdgePoint(Vector2 point){

}