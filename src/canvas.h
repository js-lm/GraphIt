#pragma once

#include "graph/graph.h"
#include "lib/raygui.h"

class Canvas{
public:
    Canvas();
    ~Canvas();

    void drawTools();
    void drawGraph() const;

    void update();
    
    void updateCanvasCamera();
    
    const Camera2D &getCanvasCamera() const{ return canvasCamera_;}

private:
    void switchMode(Graph::Mode mode);

    void changeButtonStyleTo(GuiControl target, int border, int base, int text, int borderWidth);

    Color colorFromHex(unsigned int hexValue);
    
private:
    Graph *graph_;

    Camera2D canvasCamera_;

    // for screen dragging
    Vector2 canvasCameraPivot_;
    
    bool isViewing_;
};