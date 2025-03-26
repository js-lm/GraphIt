#pragma once

enum class Setting{
    // graph settings
    GRAPH_IS_DIRECTED,          // bool
    GRAPH_IS_WEIGHTED,          // bool
    GRAPH_WEIGHT_PRECISION,     // int
    GRAPH_IS_LABELED,           // bool

    // graph visual
    GRAPH_VERTEX_RADIUS,        // float
    GRAPH_EDGE_THICKNESS,       // float

    // grid settings
    GRID_SHOW,                  // bool
    GRID_IS_SNAP_TO_GRID,       // bool
    GRID_CELL_SIZE,             // int
    GRID_SUBDIVISION_SIZE,      // int

    // color preferences
    COLOR_PREFERENCE_PICKER,    // Color

    // vertex colors
    COLOR_VERTEX_START,         // Color
    COLOR_VERTEX_END,           // Color
    COLOR_VERTEX_VISITED,       // Color
    COLOR_VERTEX_UNVISITED,     // Color
    COLOR_VERTEX_CURRENT,       // Color
    COLOR_VERTEX_SPECIAL,       // Color

    // edge colors
    COLOR_EDGE_VISITED,         // Color
    COLOR_EDGE_UNVISITED,       // Color
    COLOR_EDGE_CURRENT,         // Color
    COLOR_EDGE_PATH,            // Color
    COLOR_EDGE_BLOCKED,         // Color
    COLOR_EDGE_SPECIAL,         // Color

    // debug colors
    COLOR_DEBUG_PEN,            // Color
    COLOR_DEBUG_LINK,           // Color
    COLOR_DEBUG_DYE,            // Color

    // algorithm settings
    ALGORITHM_IS_AUTO_FORWARD,  // bool
    ALGORITHM_DROPDOWN_OPTION,  // int

    // toolbar
    TOOLBAR_CURRENT_SELECTED_TOOL, // (int)SelectedTool
    TOOLBAR_IS_SELECTING_VERTEX,   // bool
    TOOLBAR_IS_SELECTING_EDGE,     // bool
};

enum class Flag{
    // toolbar - general
    TOOLBAR_CAN_UNDO,
    TOOLBAR_CAN_REDO,

    // algorithm
    ALGORITHM_FOCUS_MODE, // this switches between two different modes
    ALGORITHM_IS_RUNNING, // this is for when you want to pause the auto forwarding

};