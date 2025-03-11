/*******************************************************************************************
*
*   Toolbar v0.0.0 - toolbar
*
*   MODULE USAGE:
*       #define GUI_TOOLBAR_IMPLEMENTATION
*       #include "gui_toolbar.h"
*
*       INIT: GuiToolbarState state = InitGuiToolbar();
*       DRAW: GuiToolbar(&state);
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

#ifndef GUI_TOOLBAR_H
#define GUI_TOOLBAR_H

typedef struct {
    Vector2 controlAnchor;
    Vector2 constructionAnchor;
    Vector2 viewAnchor;
    Vector2 generalAnchor;
    
    bool selectPressed;
    bool movePressed;
    bool penPressed;
    bool connectPressed;
    bool dragPressed;
    bool deleteSelectedPressed;
    bool eraserPressed;
    bool showGridActive;
    bool Toggle014Active;
    bool changeSelectedColorPressed;
    bool colorPanelButtonPressed;
    bool savePressed;
    bool loadPressed;
    bool newFilePressed;
    bool undoPressed;
    bool redoPressed;
    bool loadLuaPressed;

    Rectangle layoutRecs[26];

    // Custom state variables (depend on development software)
    // NOTE: This variables should be added manually if required

} GuiToolbarState;

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
GuiToolbarState InitGuiToolbar(void);
void GuiToolbar(GuiToolbarState *state);

#ifdef __cplusplus
}
#endif

#endif // GUI_TOOLBAR_H

/***********************************************************************************
*
*   GUI_TOOLBAR IMPLEMENTATION
*
************************************************************************************/
#if defined(GUI_TOOLBAR_IMPLEMENTATION)

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
GuiToolbarState InitGuiToolbar(void)
{
    GuiToolbarState state = { 0 };

    state.controlAnchor = (Vector2){ 296, 0 };
    state.constructionAnchor = (Vector2){ 0, 680 };
    state.viewAnchor = (Vector2){ 408, 680 };
    state.generalAnchor = (Vector2){ 0, 0 };
    
    state.selectPressed = false;
    state.movePressed = false;
    state.penPressed = false;
    state.connectPressed = false;
    state.dragPressed = false;
    state.deleteSelectedPressed = false;
    state.eraserPressed = false;
    state.showGridActive = true;
    state.Toggle014Active = true;
    state.changeSelectedColorPressed = false;
    state.colorPanelButtonPressed = false;
    state.savePressed = false;
    state.loadPressed = false;
    state.newFilePressed = false;
    state.undoPressed = false;
    state.redoPressed = false;
    state.loadLuaPressed = false;

    state.layoutRecs[0] = (Rectangle){ state.controlAnchor.x + 0, state.controlAnchor.y + 0, 392, 40 };
    state.layoutRecs[1] = (Rectangle){ state.controlAnchor.x + 16, state.controlAnchor.y + 8, 48, 24 };
    state.layoutRecs[2] = (Rectangle){ state.controlAnchor.x + 64, state.controlAnchor.y + 8, 72, 24 };
    state.layoutRecs[3] = (Rectangle){ state.controlAnchor.x + 144, state.controlAnchor.y + 8, 64, 24 };
    state.layoutRecs[4] = (Rectangle){ state.constructionAnchor.x + 0, state.constructionAnchor.y + 0, 408, 40 };
    state.layoutRecs[5] = (Rectangle){ state.constructionAnchor.x + 16, state.constructionAnchor.y + 8, 72, 24 };
    state.layoutRecs[6] = (Rectangle){ state.constructionAnchor.x + 96, state.constructionAnchor.y + 8, 56, 24 };
    state.layoutRecs[7] = (Rectangle){ state.constructionAnchor.x + 160, state.constructionAnchor.y + 8, 64, 24 };
    state.layoutRecs[8] = (Rectangle){ state.constructionAnchor.x + 240, state.constructionAnchor.y + 8, 64, 24 };
    state.layoutRecs[9] = (Rectangle){ state.controlAnchor.x + 224, state.controlAnchor.y + 8, 72, 24 };
    state.layoutRecs[10] = (Rectangle){ state.constructionAnchor.x + 320, state.constructionAnchor.y + 8, 72, 24 };
    state.layoutRecs[11] = (Rectangle){ state.viewAnchor.x + 0, state.viewAnchor.y + 0, 264, 40 };
    state.layoutRecs[12] = (Rectangle){ state.viewAnchor.x + 16, state.viewAnchor.y + 8, 32, 24 };
    state.layoutRecs[13] = (Rectangle){ state.viewAnchor.x + 48, state.viewAnchor.y + 8, 88, 24 };
    state.layoutRecs[14] = (Rectangle){ state.viewAnchor.x + 144, state.viewAnchor.y + 8, 104, 24 };
    state.layoutRecs[15] = (Rectangle){ state.controlAnchor.x + 312, state.controlAnchor.y + 8, 56, 24 };
    state.layoutRecs[16] = (Rectangle){ state.controlAnchor.x + 364, state.controlAnchor.y + 8, 12, 24 };
    state.layoutRecs[17] = (Rectangle){ state.generalAnchor.x + 0, state.generalAnchor.y + 0, 296, 40 };
    state.layoutRecs[18] = (Rectangle){ state.generalAnchor.x + 48, state.generalAnchor.y + 8, 24, 24 };
    state.layoutRecs[19] = (Rectangle){ state.generalAnchor.x + 80, state.generalAnchor.y + 8, 24, 24 };
    state.layoutRecs[20] = (Rectangle){ state.generalAnchor.x + 16, state.generalAnchor.y + 8, 24, 24 };
    state.layoutRecs[21] = (Rectangle){ state.generalAnchor.x + 120, state.generalAnchor.y + 8, 24, 24 };
    state.layoutRecs[22] = (Rectangle){ state.generalAnchor.x + 152, state.generalAnchor.y + 8, 24, 24 };
    state.layoutRecs[23] = (Rectangle){ state.generalAnchor.x + 192, state.generalAnchor.y + 8, 88, 24 };
    state.layoutRecs[24] = (Rectangle){ 688, 40, 312, 40 };
    state.layoutRecs[25] = (Rectangle){ 672, 720, 328, 40 };

    // Custom variables initialization

    return state;
}

void GuiToolbar(GuiToolbarState *state)
{
    GuiGroupBox(state->layoutRecs[0], NULL);
    GuiLabel(state->layoutRecs[1], "Control:");
    state->selectPressed = GuiButton(state->layoutRecs[2], "#021#Select"); 
    state->movePressed = GuiButton(state->layoutRecs[3], "#019#Move"); 
    GuiGroupBox(state->layoutRecs[4], NULL);
    GuiLabel(state->layoutRecs[5], "Construction:");
    state->penPressed = GuiButton(state->layoutRecs[6], "#022#Pen"); 
    state->connectPressed = GuiButton(state->layoutRecs[7], "#034#Link"); 
    state->dragPressed = GuiButton(state->layoutRecs[8], "#144#Drag"); 
    state->deleteSelectedPressed = GuiButton(state->layoutRecs[9], "#143#Delete"); 
    state->eraserPressed = GuiButton(state->layoutRecs[10], "#023#Eraser"); 
    GuiGroupBox(state->layoutRecs[11], NULL);
    GuiLabel(state->layoutRecs[12], "View:");
    GuiToggle(state->layoutRecs[13], "#097#Show Grid", &state->showGridActive);
    GuiToggle(state->layoutRecs[14], "#050#Snap To Grid", &state->Toggle014Active);
    state->changeSelectedColorPressed = GuiButton(state->layoutRecs[15], "#025#Dye"); 
    state->colorPanelButtonPressed = GuiButton(state->layoutRecs[16], NULL); 
    GuiGroupBox(state->layoutRecs[17], NULL);
    state->savePressed = GuiButton(state->layoutRecs[18], "#006#"); 
    state->loadPressed = GuiButton(state->layoutRecs[19], "#005#"); 
    state->newFilePressed = GuiButton(state->layoutRecs[20], "#008#"); 
    state->undoPressed = GuiButton(state->layoutRecs[21], "#072#"); 
    state->redoPressed = GuiButton(state->layoutRecs[22], "#073#"); 
    state->loadLuaPressed = GuiButton(state->layoutRecs[23], "#200#Load Lua"); 
    GuiPanel(state->layoutRecs[24], NULL);
    GuiPanel(state->layoutRecs[25], NULL);
}

#endif // GUI_TOOLBAR_IMPLEMENTATION
