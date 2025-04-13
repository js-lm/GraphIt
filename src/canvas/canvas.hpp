#pragma once

#include <raylib.h>
#include <utility>
#include <optional>
#include <unordered_set>
#include <set>
#include <functional>
#include <string>

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
        VIEW   = 0,

        SELECT = 1,
        PAN    = 2,
        PEN    = 3,
        LINK   = 4,
        MOVE   = 5,
        ERASER = 6
    };

public:
    Canvas();
    ~Canvas() = default;

    void draw() const;
    void update();

    void drawMouse() const;

    void switchMode(Mode mode);
    const Mode getMode() const{ return mode_;}
    
    const std::optional<VertexID> &getHoveredVertexID() const{ return hoveredVertexID_;}
    const std::optional<EdgeID> &getHoveredEdgeIDs() const{ return hoveredEdgeIDs_;}
    const std::unordered_set<VertexID> &getSelectedVertexIDs() const{ return selectedVertexIDs_;}
    const SelectedEdges& getSelectedEdgeIDs() const{ return selectedEdgeIDs_;}

    Vector2 getMousePositionInCanvas(bool snap = false) const;

    void resetCamera();
    void setZoomBarTimer(float second = 1.5f){ lostScrollCD_ = second;}

public:
    void doBulkDeleteVertices();
    void doBulkDeleteEdges();
    void doBulkDelete();

    void doDyeVertex();
    void doDyeEdge();
    void doDye();

public:
    Vector2 snapVector(Vector2 vector) const;

public:
    const Vector2 getCameraPosition(){ return canvasCamera_.target;}
    const float getCameraZoom(){ return canvasCamera_.zoom;}
    void setCameraPosition(Vector2 position){ canvasCamera_.target = position;}
    void setCameraZoom(float zoom);

    const std::string &getWeightInputLabel() const{ return weightInputString_;}

private:
    void drawGrid() const;

    void drawZoomProgressBar() const;

    void updateCursor() const;
    void drawPen() const;
    void drawLink() const;
    void drawEraser() const;
    void drawSelect() const;
    void drawMove() const;

    void updateHoveredItem();

    void drawMouseLabel() const;

private:
    bool isMouseOnCanvas() const;
    bool isCanvasMouseButtonPressed(int key) const;

    void updateMouseActions();

    void resetToolStatus();

    void updatePen();
    void updateLink();
    void updateEraser();
    void updateMove();
    void updateDrag();
    void updateSelect();

private:
    void handleSelectionStart();
    bool handleIndividualSelection();
    void handleGroupSelection();
    void handleEdgeWeightInput();
    void reweighSelectedEdges();

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
    std::string weightInputString_;

    // link mode
    std::optional<VertexID> linkFrom_;

    // move mode
    Vector2 pivotPoint_;
    float zoomFactorTemp_; // for integer snapping
    std::optional<float> lostScrollCD_; // for displaying the zoom factor

    // drag mode
    std::optional<VertexID> vertexToDrag_;
    Vector2 vertexOriginalPosition_;
    std::optional<float> doubleMiddleClickCD_;
    Vector2 doubleMiddleClickStartPosition_;

    // select mode
    std::optional<Vector2> startFrom_;
    std::optional<VertexID> selectedVertex_;
    std::optional<EdgeID> selectedEdge_;
};