#include "graph.h"

size_t Node::idCounter_{0};

Node::Node(Vector2 position, Color color)
    : x_(position.x), y_(position.y)
    , id_(idCounter_++)
    , color_(color)
{}

bool Node::operator==(const Node &other) const{ return id_ == other.id_;}

void Node::draw(bool isHighlighted) const{
    if(isHighlighted){
        DrawCircleV({x_, y_}, Graph::getNodeRadius() * 1.25f, color_);
    }else{
        DrawCircleV({x_, y_}, Graph::getNodeRadius(), color_);
    }
}

void Node::updatePosition(Vector2 position){
    x_ = position.x;
    y_ = position.y;
}

bool Node::contains(Vector2 position) const{
    return CheckCollisionPointCircle(position, {x_, y_}, Graph::getNodeRadius() * 2.0f);
}