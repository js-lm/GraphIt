/*******************************************************************************************
*
*   ColorPanel v0.0.0 - color
*
*   MODULE USAGE:
*       #define GUI_COLORPANEL_IMPLEMENTATION
*       #include "gui_color_panel.h"
*
*       INIT: GuiColorPanelState state = InitGuiColorPanel();
*       DRAW: GuiColorPanel(&state);
*
*   LICENSE: Propietary License
*
*   Copyright (c) 2022 xxx. All Rights Reserved.
*
*   Unauthorized copying of this file, via any medium is strictly prohibited
*   This project is proprietary and confidential unless the owner allows
*   usage in any other form by expresely written permission.
*
**********************************************************************************************/

#include "raylib.h"

// WARNING: raygui implementation is expected to be defined before including this header
#undef RAYGUI_IMPLEMENTATION
#include "lib/raygui.h"

#include <string.h>     // Required for: strcpy()

#ifndef GUI_COLORPANEL_H
#define GUI_COLORPANEL_H

typedef struct {
    bool colorPanelWindowActive;
    Color ColorPickerValue;

    // Custom state variables (depend on development software)
    // NOTE: This variables should be added manually if required

} GuiColorPanelState;

#ifdef __cplusplus
extern "C" {            // Prevents name mangling of functions
#endif

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
// ...

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
GuiColorPanelState InitGuiColorPanel(void);
void DrawGuiColorPanel(GuiColorPanelState *state, Vector2 colorAnchor, Color *referenceColor, const char *title = "Color Panel");
char *ColorToRGBString(Color color);

#ifdef __cplusplus
}
#endif

#endif // GUI_COLORPANEL_H

/***********************************************************************************
*
*   GUI_COLORPANEL IMPLEMENTATION
*
************************************************************************************/
#if defined(GUI_COLORPANEL_IMPLEMENTATION)

#include "lib/raygui.h"

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Internal Module Functions Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
GuiColorPanelState InitGuiColorPanel(void)
{
    GuiColorPanelState state = { 0 };
    
    state.colorPanelWindowActive = true;
    state.ColorPickerValue = (Color){ 0, 0, 0, 0 };

    // Custom variables initialization

    return state;
}

void DrawGuiColorPanel(GuiColorPanelState *state, Vector2 colorAnchor, Color *referenceColor, const char *title)
{
    if (state->colorPanelWindowActive)
    {
        state->colorPanelWindowActive = !GuiWindowBox((Rectangle){ colorAnchor.x + 0, colorAnchor.y + 0, 136, 152 }, title);
        GuiColorPicker((Rectangle){ colorAnchor.x + 8, colorAnchor.y + 32, 96, 96 }, NULL, referenceColor);
        GuiLabel((Rectangle){ colorAnchor.x + 8, colorAnchor.y + 128, 32, 24 }, "RGB:");
        GuiLabel((Rectangle){ colorAnchor.x + 40, colorAnchor.y + 128, 88, 24 }, ColorToRGBString(*referenceColor));
    }
}

char *ColorToRGBString(Color color)
{
    static char buffer[16];
    sprintf(buffer, "%3d  %3d  %3d", color.r, color.g, color.b);
    return buffer;
}

#endif // GUI_COLORPANEL_IMPLEMENTATION
