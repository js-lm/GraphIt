#pragma once

#include <raylib.h>
#include <cstddef>

namespace GraphNamespace{

    class Vertex{
    public:
        Vertex() = delete;
        ~Vertex() = default;
        Vertex(Vector2 position, Color color, size_t id)
            : position_(position)
            , color_(color)
            , id_(id)
            , isHidden_(false)
        {};

        void hide(){ isHidden_ = true;};
        void show(){ isHidden_ = false;};

        void setPosition(Vector2 position){ position_ = position;};
        void setColor(Color color){ color_ = color;};

    public:
        bool operator==(const Vertex &other) const{ return id_ == other.id_;};

        const float x() const{ return position_.x;};
        const float y() const{ return position_.y;};
        const Vector2 position() const{ return position_;};
        const Color color() const{return color_;};
        const size_t id() const{ return id_;};
        const bool isHidden() const{ return isHidden_;};

    private:
        Vector2 position_;
        Color color_;
        size_t id_;
        bool isHidden_;
    };

    class Edge{
    public:
        Edge() = delete;
        ~Edge() = default;
        Edge(size_t startID, size_t endID, Color color)
            : startID_(startID)
            , endID_(endID)
            , color_(color)
        {};

        void setColor(Color color){ color_ = color;};

    public:
        const size_t startID() const{ return startID_;};
        const size_t endID() const{ return endID_;};
        const bool contains(size_t id) const{ return startID_ == id || endID_ == id;};
        const Color color() const{ return color_;};

    private:
        size_t startID_;
        size_t endID_;

        Color color_;
    };

} // namespace GraphNamespace