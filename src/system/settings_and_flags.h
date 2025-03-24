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
    GRID_IS_ENABLED,            // bool
    GRID_IS_SNAP_TO_GRID,       // bool
    GRID_CELL_SIZE,             // int
    GRID_SUBDIVISION_SIZE,      // int

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

    // algorithm settings
    ALGORITHM_IS_AUTO_FORWARD,  // bool
};

enum class Flag{
    // canvas
    CANVAS_SELECTED_TOOL,       // (int) Selected Tool
    CANVAS_IS_UI_HIDDEN,        // bool

    // algorithm settings
    ALGORITHM_IS_RUNNING,       // bool
    ALGORITHM_TYPE,             // (int) Algorithm Type
};