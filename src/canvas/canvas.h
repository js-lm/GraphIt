#pragma once

#include <raylib.h>
#include <utility>
#include <optional>
#include <unordered_set>
#include <set>
#include <functional>

class Canvas{
private:
    struct PairHash{
        template <class Type1, class Type2>
        size_t operator()(const std::pair<Type1, Type2> &inputPair) const{
            auto hashFirst{std::hash<Type1>{}(inputPair.first)};
            auto hashSecond{std::hash<Type2>{}(inputPair.second)};
            return hashFirst ^ (hashSecond << 1);
        }
    };

public:
    using VertexID = size_t;

    using SelectedVertices = std::unordered_set<VertexID>;
    
    using EdgeID = std::pair<VertexID, VertexID>;
    using SelectedEdges = std::unordered_set<EdgeID, PairHash>;

public:
    enum class Mode{
        VIEW,
        SELECT,
        MOVE,
        PEN,
        LINK,
        DRAG,
        ERASER
    };

public:
    Canvas();
    ~Canvas() = default;

    void draw() const;
    void update();

    void switchMode(Mode mode);
    const Mode getMode() const{ return mode_;};
    
    const std::optional<VertexID> &getHoveredVertexID() const{ return hoveredVertexID_;};
    const std::optional<EdgeID> &getHoveredEdgeIDs() const{ return hoveredEdgeIDs_;};
    const std::unordered_set<VertexID> &getSelectedVertexIDs() const{ return selectedVertexIDs_;};
    const SelectedEdges& getSelectedEdgeIDs() const{ return selectedEdgeIDs_;};

    Vector2 getMousePositionInCanvas() const;

    void resetCamera();
    
private:
    void drawGrid() const;

    void drawPen() const;
    void drawLink() const;

    void updateHoveredItem();

private:
    void updateMouseActions();
    void updateKeyboardActions();

    void resetToolStatus();

    void updatePen();
    void updateLink();
    void updateEraser();

private:
    Camera2D canvasCamera_;

    Mode mode_;

    std::optional<VertexID> hoveredVertexID_;
    std::optional<EdgeID> hoveredEdgeIDs_;

    SelectedVertices selectedVertexIDs_;
    SelectedEdges selectedEdgeIDs_;

    // link mode
    std::optional<VertexID> linkFrom_;
};