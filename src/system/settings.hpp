#pragma once

#include <raylib.h>

struct AppSettings {
    // Graph settings
    bool graphIsDirected{true};
    bool graphIsWeighted{true};
    int graphWeightPrecision{2};
    bool graphIsLabeled{true};

    // Graph visual
    float graphVertexRadius{20.0f};
    float graphEdgeThickness{7.0f};

    // Grid settings
    bool gridShow{true};
    bool gridIsSnapToGrid{true};
    int gridCellSize{24};
    int gridSubdivisionSize{3};

    // Color preferences
    Color colorPreferencePicker{137, 207, 240, 255};

    // Vertex colors
    Color colorVertexStart{180, 223, 255, 255}; // light blue
    Color colorVertexEnd{180, 223, 255, 255}; // same as above
    Color colorVertexVisited{79, 176, 159, 255}; // green
    Color colorVertexUnvisited{134, 161, 219, 255}; // blue
    Color colorVertexCurrent{255, 8, 74, 255}; // red
    Color colorVertexPivot{255, 163, 191, 255}; // pink

    // Edge colors
    Color colorEdgeVisited{136, 216, 176, 255}; // mint
    Color colorEdgeUnvisited{255, 238, 173, 255}; // rice?
    Color colorEdgeCurrent{255, 111, 105, 255}; // dark orange
    Color colorEdgePath{BLACK};
    Color colorEdgeBlocked{BLACK};
    Color colorEdgeSpecial{BLACK};

    // Debug colors
    Color colorDebugPen{colorVertexUnvisited};
    Color colorDebugLink{colorEdgeUnvisited};
    Color colorDebugDye{colorVertexCurrent};
    int colorPanelCallFrom{0}; // none:0 pen:1 link:2 dye:3

    // Algorithm settings
    bool algorithmIsAutoForward{true};
    int algorithmDropdownOption{0};
    float autoForwardSecondPerStep{.25f};

    // Toolbar
    int toolbarCurrentSelectedTool{0}; // SelectedTool
    bool toolbarIsSelectingVertex{true};
    bool toolbarIsSelectingEdge{true};
};

struct AppFlags{
    // Toolbar - general
    bool toolbarCanUndo{false};
    bool toolbarCanRedo{false};

    // Algorithm
    bool algorithmFocusMode{false};
    bool algorithmIsRunning{false};
};

extern AppSettings appSettings;
extern AppFlags appFlags;