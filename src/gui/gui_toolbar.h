/*******************************************************************************************
*   MODULE USAGE:
*       #define GUI_TOOLBAR_IMPLEMENTATION
*       #include "gui_toolbar.h"
*
*       INIT: GuiToolbarState state = InitGuiToolbar();
*       DRAW: GuiToolbar(&state);
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
    Vector2 settingAnchor;
    
    // File
    bool newFilePressed;
    bool savePressed;
    bool loadPressed;
    bool undoPressed;
    bool redoPressed;
    bool takeScreenshotPressed;
    bool loadLuaPressed;

    // View
    bool showGridActive;
    bool snapToGridActive;

    // Settings
    bool isDirectedChecked;
    bool isWeightedChecked;
    bool isShowingLabelsChecked;
    bool openSettingPressed;

    // Construction
    bool penPressed;
    bool penColorPressed;
    bool linkPressed;
    bool linkColorPressed;
    bool dragPressed;
    bool eraserPressed;

    // Controls
    bool selectPressed;
    bool movePressed;
    bool deleteSelectedPressed;
    bool changeSelectedColorPressed;
    bool colorPanelButtonPressed;
    bool isSelectingVertexChecked;
    bool isSelectingEdgeChecked;
} GuiToolbarState;

#ifdef __cplusplus
extern "C" {            // Prevents name mangling of functions
#endif

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
// Module Functions Definition
//----------------------------------------------------------------------------------
GuiToolbarState InitGuiToolbar(void)
{
    GuiToolbarState state = { 0 };

    state.controlAnchor = (Vector2){ 400, 680 };
    state.constructionAnchor = (Vector2){ 0, 680 };
    state.viewAnchor = (Vector2){ 320, 0 };
    state.generalAnchor = (Vector2){ 0, 0 };
    state.settingAnchor = (Vector2){ 584, 0 };
    
    // File
    state.newFilePressed = false;
    state.savePressed = false;
    state.loadPressed = false;
    state.undoPressed = false;
    state.redoPressed = false;
    state.takeScreenshotPressed = false;
    state.loadLuaPressed = false;

    // View
    state.showGridActive = true;
    state.snapToGridActive = true;

    // Settings
    state.isDirectedChecked = false;
    state.isWeightedChecked = false;
    state.isShowingLabelsChecked = false;
    state.openSettingPressed = false;

    // Construction
    state.penPressed = false;
    state.penColorPressed = false;
    state.linkPressed = false;
    state.linkColorPressed = false;
    state.dragPressed = false;
    state.eraserPressed = false;

    // Controls
    state.selectPressed = false;
    state.movePressed = false;
    state.deleteSelectedPressed = false;
    state.changeSelectedColorPressed = false;
    state.colorPanelButtonPressed = false;
    state.isSelectingVertexChecked = true;
    state.isSelectingEdgeChecked = false;

    return state;
}

void GuiToolbar(GuiToolbarState *state)
{
    // File
    GuiGroupBox((Rectangle){ state->generalAnchor.x + 0, state->generalAnchor.y + 0, 320, 40 }, NULL);
    state->newFilePressed = GuiButton((Rectangle){ state->generalAnchor.x + 16, state->generalAnchor.y + 8, 24, 24 }, "#008#");
    state->loadPressed = GuiButton((Rectangle){ state->generalAnchor.x + 44, state->generalAnchor.y + 8, 24, 24 }, "#005#");
    state->savePressed = GuiButton((Rectangle){ state->generalAnchor.x + 72, state->generalAnchor.y + 8, 24, 24 }, "#006#");
    state->undoPressed = GuiButton((Rectangle){ state->generalAnchor.x + 112, state->generalAnchor.y + 8, 24, 24 }, "#072#");
    state->redoPressed = GuiButton((Rectangle){ state->generalAnchor.x + 140, state->generalAnchor.y + 8, 24, 24 }, "#073#");
    state->takeScreenshotPressed = GuiButton((Rectangle){ state->generalAnchor.x + 180, state->generalAnchor.y + 8, 24, 24 }, "#183#");
    state->loadLuaPressed = GuiButton((Rectangle){ state->generalAnchor.x + 220, state->generalAnchor.y + 8, 88, 24 }, "#200#Load Lua");
    
    // View
    GuiGroupBox((Rectangle){ state->viewAnchor.x + 0, state->viewAnchor.y + 0, 264, 40 }, NULL);
    GuiLabel((Rectangle){ state->viewAnchor.x + 16, state->viewAnchor.y + 8, 32, 24 }, "View:");
    GuiToggle((Rectangle){ state->viewAnchor.x + 48, state->viewAnchor.y + 8, 88, 24 }, "#097#Show Grid", &state->showGridActive);
    GuiToggle((Rectangle){ state->viewAnchor.x + 140, state->viewAnchor.y + 8, 104, 24 }, "#050#Snap To Grid", &state->snapToGridActive);
    
    // Settings
    GuiGroupBox((Rectangle){ state->settingAnchor.x + 0, state->settingAnchor.y + 0, 416, 40 }, NULL);
    GuiLabel((Rectangle){ state->settingAnchor.x + 16, state->settingAnchor.y + 8, 48, 24 }, "Settings:");
    GuiCheckBox((Rectangle){ state->settingAnchor.x + 72, state->settingAnchor.y + 8, 24, 24 }, "Is Directed", &state->isDirectedChecked);
    GuiCheckBox((Rectangle){ state->settingAnchor.x + 168, state->settingAnchor.y + 8, 24, 24 }, "Is Weighted", &state->isWeightedChecked);
    GuiCheckBox((Rectangle){ state->settingAnchor.x + 264, state->settingAnchor.y + 8, 24, 24 }, "Show Label", &state->isShowingLabelsChecked);
    state->openSettingPressed = GuiButton((Rectangle){ state->settingAnchor.x + 376, state->settingAnchor.y + 8, 24, 24 }, "#142#");
    
    // Construction
    GuiGroupBox((Rectangle){ state->constructionAnchor.x + 0, state->constructionAnchor.y + 0, 400, 40 }, NULL);
    GuiLabel((Rectangle){ state->constructionAnchor.x + 16, state->constructionAnchor.y + 8, 72, 24 }, "Construction:");
    state->penColorPressed = GuiButton((Rectangle){ state->constructionAnchor.x + 150, state->constructionAnchor.y + 8, 12, 24 }, NULL);
    state->penPressed = GuiButton((Rectangle){ state->constructionAnchor.x + 96, state->constructionAnchor.y + 8, 56, 24 }, "#022#Pen");
    state->linkColorPressed = GuiButton((Rectangle){ state->constructionAnchor.x + 220, state->constructionAnchor.y + 8, 12, 24 }, NULL);
    state->linkPressed = GuiButton((Rectangle){ state->constructionAnchor.x + 167, state->constructionAnchor.y + 8, 56, 24 }, "#034#Link");
    state->dragPressed = GuiButton((Rectangle){ state->constructionAnchor.x + 248, state->constructionAnchor.y + 8, 64, 24 }, "#144#Drag");
    state->eraserPressed = GuiButton((Rectangle){ state->constructionAnchor.x + 315, state->constructionAnchor.y + 8, 72, 24 }, "#023#Eraser");
    
    // Controls
    GuiGroupBox((Rectangle){ state->controlAnchor.x + 0, state->controlAnchor.y + 0, 600, 40 }, NULL);
    GuiLabel((Rectangle){ state->controlAnchor.x + 16, state->controlAnchor.y + 8, 48, 24 }, "Control:");
    state->selectPressed = GuiButton((Rectangle){ state->controlAnchor.x + 64, state->controlAnchor.y + 8, 72, 24 }, "#021#Select");
    state->movePressed = GuiButton((Rectangle){ state->controlAnchor.x + 140, state->controlAnchor.y + 8, 64, 24 }, "#019#Move");
    state->colorPanelButtonPressed = GuiButton((Rectangle){ state->controlAnchor.x + 350, state->controlAnchor.y + 8, 12, 24 }, NULL);
    state->changeSelectedColorPressed = GuiButton((Rectangle){ state->controlAnchor.x + 296, state->controlAnchor.y + 8, 56, 24 }, "#025#Dye");
    state->deleteSelectedPressed = GuiButton((Rectangle){ state->controlAnchor.x + 221, state->controlAnchor.y + 8, 72, 24 }, "#143#Delete");
    GuiLabel((Rectangle){ state->controlAnchor.x + 384, state->controlAnchor.y + 8, 40, 24 }, "Include:");
    GuiCheckBox((Rectangle){ state->controlAnchor.x + 432, state->controlAnchor.y + 8, 24, 24 }, "Node", &state->isSelectingVertexChecked);
    GuiCheckBox((Rectangle){ state->controlAnchor.x + 496, state->controlAnchor.y + 8, 24, 24 }, "Edge", &state->isSelectingEdgeChecked);
}

#endif // GUI_TOOLBAR_IMPLEMENTATION
