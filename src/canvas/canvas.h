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

    void drawMouse() const;

    void switchMode(Mode mode);
    const Mode getMode() const{ return mode_;};
    
    const std::optional<VertexID> &getHoveredVertexID() const{ return hoveredVertexID_;};
    const std::optional<EdgeID> &getHoveredEdgeIDs() const{ return hoveredEdgeIDs_;};
    const std::unordered_set<VertexID> &getSelectedVertexIDs() const{ return selectedVertexIDs_;};
    const SelectedEdges& getSelectedEdgeIDs() const{ return selectedEdgeIDs_;};

    Vector2 getMousePositionInCanvas() const;

    void resetCamera();

public:
    void doBulkDeleteVertices();
    void doBulkDeleteEdges();
    void doBulkDelete();

private:
    void drawGrid() const;

    void drawPen() const;
    void drawLink() const;
    void drawEraser() const;
    void drawSelect() const;

    void updateHoveredItem();

private:
    bool isCanvasMouseButtonPressed(int key);

    void updateMouseActions();

    void resetToolStatus();

    void updatePen();
    void updateLink();
    void updateEraser();
    void updateMove();
    void updateDrag();
    void updateSelect();

private:
    void updateCamera();
    void updateScreenDragging(int key = 2);
    void updateScreenReset();
    void updateScreenZooming();

private:
    Rectangle normalizeRectangle(Vector2 startingPosition, Vector2 endingPosition) const;

private:
    Camera2D canvasCamera_;

    Mode mode_;

    std::optional<VertexID> hoveredVertexID_;
    std::optional<EdgeID> hoveredEdgeIDs_;

    SelectedVertices selectedVertexIDs_;
    SelectedEdges selectedEdgeIDs_;

    // link mode
    std::optional<VertexID> linkFrom_;

    // move mode
    Vector2 pivotPoint_;

    // drag mode
    std::optional<VertexID> vertexToDrag_;
    Vector2 vertexOriginalPosition_;
    std::optional<float> doubleMiddleClickCD_;

    // select mode
    std::optional<Vector2> startFrom_;
};