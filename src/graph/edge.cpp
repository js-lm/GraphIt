#include "graph.h"

size_t Edge::idCounter_{0};

Edge::Edge(Node *n1, Node *n2)
    : start_(n1), end_(n2)
    , id_(idCounter_++)
{}

void Edge::draw(bool isHighlighted) const{
    if(isHighlighted){
        DrawLineEx(start_->position(), end_->position(), Graph::getEdgeThickness() * 1.25f, Graph::getEdgeColor());
    }else{
        DrawLineEx(start_->position(), end_->position(), Graph::getEdgeThickness(), Graph::getEdgeColor());
    }
}

bool Edge::contains(const Node &node) const{
    return start_ == &node || end_ == &node;
}

bool Edge::contains(Vector2 position) const{
    return CheckCollisionPointLine(position, start_->position(), end_->position(), Graph::getEdgeThickness() * 2.0f);
}

bool Edge::isEqual(Node *start, Node *end, bool isDirected) const{
    return (start_ == start && end_ == end) || (!isDirected && start_ == end && end_ == start);
}