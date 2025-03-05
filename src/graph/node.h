#pragma once

#include <stddef.h>
#include <raylib.h>
#include <iostream>

struct Node{
public:
    Node() = delete;
    Node(Vector2 position, Color color);
    ~Node() = default;

    bool operator==(const Node &other) const;

    void draw(bool isHighlighted = false) const;
    void updatePosition(Vector2 position);

    bool contains(Vector2 position) const;
    
    const float x() const{ return x_;};
    const float y() const{ return y_;};

    const Vector2 position() const{ return {x_, y_};};

    const size_t id() const{ return id_;};

private:
    float x_;
    float y_;
    
    size_t id_;

    Color color_;

private:
    static size_t idCounter_;
};
