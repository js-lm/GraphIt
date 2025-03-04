#include "graph.h"

size_t Node::idCounter_{0};

Node::Node(Vector2 position)
    : x_(position.x), y_(position.y)
    , id_(idCounter_++)
{}

bool Node::operator==(const Node &other) const{ return id_ == other.id_;}

void Node::draw(bool isHighlighted) const{
    if(isHighlighted){
        DrawCircleV({x_, y_}, Graph::getNodeRadius() * 1.25f, Graph::getNodeColor());
    }else{
        DrawCircleV({x_, y_}, Graph::getNodeRadius(), Graph::getNodeColor());
    }
}

void Node::updatePosition(Vector2 position){
    x_ = position.x;
    y_ = position.y;
}

bool Node::contains(Vector2 position) const{
    return CheckCollisionPointCircle(position, {x_, y_}, Graph::getNodeRadius() * 2.0f);
}