#pragma once

#include "node.h"

struct Edge{
public:
    Edge() = delete;
    Edge(Node *n1, Node *n2, Color color);
    ~Edge() = default;

    void draw(bool isHighlighted = false) const;

    bool contains(const Node &node) const;
    bool contains(Vector2 position) const;

    bool isEqual(Node *n1, Node *n2, bool isDirected) const;

    size_t id() const{ return id_;};
    
private:
    Node *start_;
    Node *end_;

    size_t id_;

    Color color_;

private:
    static size_t idCounter_;
};
