#pragma once

#include "node.h"

struct Edge{
public:
    Edge() = delete;
    Edge(Node *n1, Node *n2);
    ~Edge() = default;

    void draw(bool isHighlighted = false) const;

    bool contains(const Node &node) const;
    bool contains(Vector2 position) const;

    bool isEqual(Node *n1, Node *n2, bool isDirected) const;

    size_t id() const{ return id_;};
    
private:
    Node *start_;
    Node *end_;

    float thickness_{10};
    Color color_{199, 160, 210, 255};

    size_t id_;

private:
    static size_t idCounter_;
};
