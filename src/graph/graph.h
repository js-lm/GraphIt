#pragma once

#include "edge.h"

#include <vector>
#include <algorithm>

class Graph{
public:
    enum class Mode{
        EDIT, // default mode
        ADD,
        DRAG,
        CONNECT,
        DEL_NODE,
        DEL_EDGE
    };

public:
    Graph();
    Graph(const std::vector<Node*> &nodes, const std::vector<Edge*> &edges);
    ~Graph();

    void draw() const;
    void update();

    void switchMode(Mode mode);
    void reset();

    Mode getMode() const{ return mode_;};

    static Color getNodeColor(){ return defaultNodeColor_;}
    static Color getEdgeColor(){ return defaultEdgeColor_;}
    static float getNodeRadius(){ return nodeRadius_;}
    static float getEdgeThickness(){ return edgeThickness_;}

private:
    Node *findNode(Vector2 position);
    Edge *findEdge(Vector2 position);

    void updateHighlightedNodeAndEdge();

    void updateAddMode();
    void updateDragMode();
    void updateConnectMode();
    void updateDeleteNodeMode();
    void updateDeleteEdgeMode();

    void addNode(Vector2 position);
    void removeNode(Node *node);
    bool addEdge(Node *n1, Node *n2);
    void removeEdge(Edge *edge);

private:
    std::vector<Node*> nodes_;
    std::vector<Edge*> edges_;

    bool isDirected_;

    Mode mode_;

    Node *highlightedNode_;
    Edge *highlightedEdge_;

    // CONNECT mode
    Node *connectFrom_;

    // DRAG mode
    Node *draggedNode_;

    static constexpr Color defaultNodeColor_{220, 144, 169, 255};
    static constexpr Color defaultEdgeColor_{199, 160, 210, 255};
    static constexpr float nodeRadius_{10};
    static constexpr float edgeThickness_{10};
};