#pragma once

#include "graph/graph.h"
#include "lib/raygui.h"

class Canvas{
public:
    Canvas() = default;

    void drawTools();
    void drawGraph() const;

    void update();

private:
    void switchMode(Graph::Mode mode);

    void changeButtonStyleTo(GuiControl target, int border, int base, int text, int borderWidth);

    Color colorFromHex(unsigned int hexValue);

private:
    Graph graph_;

    bool isViewing_{false};
};