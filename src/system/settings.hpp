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
    Color colorVertexStart{GREEN};
    Color colorVertexEnd{RED};
    Color colorVertexVisited{BLUE};
    Color colorVertexUnvisited{LIGHTGRAY};
    Color colorVertexCurrent{YELLOW};
    Color colorVertexSpecial{PURPLE};

    // Edge colors
    Color colorEdgeVisited{BLUE};
    Color colorEdgeUnvisited{LIGHTGRAY};
    Color colorEdgeCurrent{YELLOW};
    Color colorEdgePath{GREEN};
    Color colorEdgeBlocked{RED};
    Color colorEdgeSpecial{PURPLE};

    // Debug colors
    Color colorDebugPen{DARKGRAY};
    Color colorDebugLink{GRAY};
    Color colorDebugDye{MAROON};
    int colorPanelCallFrom{0}; // none:0 pen:1 link:2 dye:3

    // Algorithm settings
    bool algorithmIsAutoForward{true};
    int algorithmDropdownOption{0};

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