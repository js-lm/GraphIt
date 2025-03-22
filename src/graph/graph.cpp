#include "graph.h"

#include <raylib.h>

Graph::Graph()
    : isDirected_(false)
    , isWeighted_(false)
    , defaultVertexColor_(BLACK)
    , defaultEdgeWeight_(1.0f)
    , defaultEdgeColor_(GRAY)
    , vertexRadius_(20.0f)
    , edgeThickness_(7.0f)  
{}

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


bool Graph::connectVertices(VertexID startID, VertexID endID, Color color){
    return connectVertices(startID, endID, std::nullopt, color);
}
bool Graph::connectVertices(VertexID startID, VertexID endID, std::optional<float> weight, std::optional<Color> color){
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
            weight.value_or(defaultEdgeWeight_),
            color.value_or(defaultEdgeColor_)
        )
    );

    return true;
}

std::pair<float, Color> Graph::disconnectVertices(VertexID startID, VertexID endID){
    std::pair<float, Color> properties;

    if(!areNeighbors(startID, endID)) return properties;

    std::vector<std::unique_ptr<Edge>> newEdges_;
    newEdges_.reserve(edges_.size());

    for(auto &edge : edges_){
        if(isTheSameEdge(edge->startID(), edge->endID(), startID, endID)){
            properties.first = edge->weight();
            properties.second = edge->color();
            continue;
        }

        newEdges_.emplace_back(std::move(edge));
    }
    
    edges_ = std::move(newEdges_);

    return properties;
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

std::optional<Graph::VertexID> Graph::findVertex(Vector2 point, std::optional<float> radius){
    if(!radius) radius = vertexRadius_;
    for(auto i{vertices_.size()}; i--> 0;){
        if(!vertices_[i]->isHidden() 
        && CheckCollisionPointCircle(point, vertices_[i]->position(), radius.value())
        ){
            return vertices_[i]->id();
        }
    }

    return std::nullopt;
}

std::optional<Graph::EdgeID> Graph::findEdge(Vector2 point, std::optional<float> thickness){
    if(!thickness) thickness = edgeThickness_;
    for(auto i{edges_.size()}; i--> 0;){
        if(!isVertexHidden(edges_[i]->startID())
        && !isVertexHidden(edges_[i]->endID())
        && CheckCollisionPointLine(
                point, 
                vertices_[edges_[i]->startID()]->position(), 
                vertices_[edges_[i]->endID()]->position(),
                thickness.value()
            )
        ){
            return std::make_pair(edges_[i]->startID(), edges_[i]->endID());
        }
    }

    return std::nullopt;
}

std::vector<Graph::VertexID> Graph::findVertex(Rectangle area){
    std::vector<VertexID> verticesFound;

    for(const auto &vertex : vertices_){
        if(vertex->isHidden()) continue;
        if(CheckCollisionPointRec(vertex->position(), area)){
            verticesFound.emplace_back(vertex->id());
        }
    }

    return verticesFound;
}

std::vector<Graph::EdgeID> Graph::findEdge(Rectangle area){
    std::vector<EdgeID> edgesFound;

    for(const auto &edge : edges_){
        if((!isVertexHidden(edge->startID()) && !isVertexHidden(edge->endID()))
        && checkCollisionLineRectangle(
            vertices_[edge->startID()]->position(),
            vertices_[edge->endID()]->position(),
            area
           )
        ){
            edgesFound.emplace_back(std::pair{edge->startID(), edge->endID()});
        }
    }

    return edgesFound;
}

bool Graph::isTheSameEdge(VertexID start1, VertexID end1, VertexID start2, VertexID end2) const{
    return ((start1 == start2
            && end1 == end2)
           || (!isDirected_
            && start1 == end2
            && end1 == start2
           ));
}

bool Graph::checkCollisionLineRectangle(Vector2 start, Vector2 end, Rectangle rectangle) const{
    Vector2 topLeft{rectangle.x, rectangle.y};
    Vector2 topRight{rectangle.x + rectangle.width, rectangle.y};
    Vector2 bottomLeft{rectangle.x, rectangle.y + rectangle.height};
    Vector2 bottomRight{rectangle.x + rectangle.width, rectangle.y + rectangle.height};

    Vector2 collisionPoint;
    
    return CheckCollisionPointRec(start, rectangle)
        || CheckCollisionPointRec(end, rectangle)
        || CheckCollisionLines(start, end, topLeft, topRight, &collisionPoint)
        || CheckCollisionLines(start, end, topRight, bottomRight, &collisionPoint)
        || CheckCollisionLines(start, end, bottomRight, bottomLeft, &collisionPoint)
        || CheckCollisionLines(start, end, bottomLeft, topLeft, &collisionPoint);
}

std::vector<Color> Graph::dyeSelectedVertices(const std::vector<VertexID> &ids, Color newColor){
    std::vector<Color> colors;

    for(const auto &id : ids){
        if(isVertexHidden(id)) continue;
        colors.emplace_back(vertices_[id]->color());
        vertices_[id]->setColor(newColor);
    }

    return colors;
}

std::vector<Color> Graph::dyeSelectedEdge(const std::vector<EdgeID> &ids, Color newColor){
    std::vector<Color> colors;
    for(const auto &id : ids){
        for(const auto &edge : edges_){
            if(isTheSameEdge(id.first, id.second, edge->startID(), edge->endID())){
                colors.emplace_back(edge->color());
                edge->setColor(newColor);
                continue;
            }
        }
    }
    return colors;
}

void Graph::dyeSelectedVertices(const std::vector<VertexID> &ids, const std::vector<Color> &newColor){
    for(size_t i{0}; i < ids.size(); i++){
        vertices_[ids[i]]->setColor(newColor[i]);
    }
}

void Graph::dyeSelectedEdge(const std::vector<EdgeID> &ids, const std::vector<Color> &newColor){
    for(size_t i{0}; i < ids.size(); i++){
        for(const auto &edge : edges_){
            if(isTheSameEdge(ids[i].first, ids[i].second, edge->startID(), edge->endID())){
                edge->setColor(newColor[i]);
                continue;
            }
        }
    }
}

std::string Graph::getLabel(VertexID id) const{
    if(id == 0) return "A";
    std::string newLabel;
    while(id > 0){
        newLabel = static_cast<char>('A' + id % 26) + newLabel;
        id /= 26;
    }
    return newLabel;
}