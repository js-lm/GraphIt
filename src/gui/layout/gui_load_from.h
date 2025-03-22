/*******************************************************************************************
*
*   LoadFrom v69999 - Tool Description
*
*   MODULE USAGE:
*       #define GUI_LOAD_FROM_IMPLEMENTATION
*       #include "gui_load_from.h"
*
*       INIT: GuiLoadFromState state = InitGuiLoadFrom();
*       DRAW: GuiLoadFrom(&state);
*
*   LICENSE: Propietary License
*
*   Copyright (c) 2022 raylib technologies. All Rights Reserved.
*
*   Unauthorized copying of this file, via any medium is strictly prohibited
*   This project is proprietary and confidential unless the owner allows
*   usage in any other form by expresely written permission.
*
**********************************************************************************************/

#include "raylib.h"

// WARNING: raygui implementation is expected to be defined before including this header
#undef RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <string.h>     // Required for: strcpy()

#ifndef GUI_LOAD_FROM_H
#define GUI_LOAD_FROM_H

typedef struct {
    Vector2 anchor;
    
    bool loadFileDialogActive;
    bool directoryBackPressed;
    bool pathEditMode;
    char pathText[128];
    bool fileNameEditMode;
    char fileNameText[128];
    bool fileSelectPressed;
    bool fileCancelPressed;
    bool fileTypeEditMode;
    int fileTypeActive;
    Rectangle ScrollPanelScrollView;
    Vector2 ScrollPanelScrollOffset;
    Vector2 ScrollPanelBoundsOffset;

    // Custom state variables (depend on development software)
    // NOTE: This variables should be added manually if required

} GuiLoadFromState;

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
GuiLoadFromState InitGuiLoadFrom(void);
void GuiLoadFrom(GuiLoadFromState *state);
// void DrawGuiLoadScrollPanel(GuiLoadFromState *state);

#ifdef __cplusplus
}
#endif

#endif // GUI_LOAD_FROM_H

/***********************************************************************************
*
*   GUI_LOAD_FROM IMPLEMENTATION
*
************************************************************************************/
#if defined(GUI_LOAD_FROM_IMPLEMENTATION)

#include "raygui.h"

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
GuiLoadFromState InitGuiLoadFrom(void)
{
    GuiLoadFromState state = { 0 };

    // 480, 300 
    state.anchor = (Vector2){(float)(GetScreenWidth() + 480) / 2.0f, (float)(GetScreenHeight() + 300) / 2.0f};
    
    state.loadFileDialogActive = false;
    state.directoryBackPressed = false;
    state.pathEditMode = false;
    strcpy(state.pathText, "GRT");
    state.fileNameEditMode = false;
    strcpy(state.fileNameText, "save.grt");
    state.fileSelectPressed = false;
    state.fileCancelPressed = false;
    state.fileTypeEditMode = false;
    state.fileTypeActive = 0;
    state.ScrollPanelScrollView = (Rectangle){ 0, 0, 0, 0 };
    state.ScrollPanelScrollOffset = (Vector2){ 0, 0 };
    state.ScrollPanelBoundsOffset = (Vector2){ 0, 0 };

    // Custom variables initialization

    return state;
}

void DrawGuiLoadPanel(GuiLoadFromState *state)
{
    if (state->fileTypeEditMode) GuiLock();

    if (state->loadFileDialogActive)
    {
        state->loadFileDialogActive = !GuiWindowBox((Rectangle){ state->anchor.x + 0, state->anchor.y + 0, 480, 300 }, "#005#Select a svae file");
        state->directoryBackPressed = GuiButton((Rectangle){ state->anchor.x + 424, state->anchor.y + 32, 48, 24 }, "<"); 
        if (GuiTextBox((Rectangle){ state->anchor.x + 8, state->anchor.y + 32, 408, 24 }, state->pathText, 128, state->pathEditMode)) state->pathEditMode = !state->pathEditMode;
        GuiLabel((Rectangle){ state->anchor.x + 8, state->anchor.y + 232, 64, 24 }, "File name:");
        if (GuiTextBox((Rectangle){ state->anchor.x + 72, state->anchor.y + 232, 280, 24 }, state->fileNameText, 128, state->fileNameEditMode)) state->fileNameEditMode = !state->fileNameEditMode;
        state->fileSelectPressed = GuiButton((Rectangle){ state->anchor.x + 360, state->anchor.y + 232, 112, 24 }, "#005#Select"); 
        state->fileCancelPressed = GuiButton((Rectangle){ state->anchor.x + 360, state->anchor.y + 264, 112, 24 }, "##Cancel"); 
        GuiLabel((Rectangle){ state->anchor.x + 8, state->anchor.y + 264, 64, 24 }, "File filter:");
        if (GuiDropdownBox((Rectangle){ state->anchor.x + 72, state->anchor.y + 264, 280, 24 }, "GraphIt! (.grt);All (*.*)", &state->fileTypeActive, state->fileTypeEditMode)) state->fileTypeEditMode = !state->fileTypeEditMode;

        // DrawGuiLoadScrollPanel(state);
    }
    
    GuiUnlock();
}


#endif // GUI_LOAD_FROM_IMPLEMENTATION
