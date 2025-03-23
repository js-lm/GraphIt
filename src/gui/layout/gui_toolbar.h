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
#include "raygui.h"

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

    // View
    bool showGridActive;
    bool snapToGridActive;

    // Settings
    bool openSettingPressed;
    bool algorithmDropdownBoxEditMode;
    int algorithmDropdownBoxActive;
    bool runAlgorithmButtonPressed;
    bool goToPreviousStepPressed;
    bool goToNextStepPressed;
    bool autoForwardActive;
    bool isHoldingUIActive;
    bool isHiddenUiDecorativeButtonPressed;
    bool exitAlgorithmPressed;

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

    // Others
    int currentSelectedTool;
    bool canUndo;
    bool canRedo;

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

#include "raygui.h"

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
GuiToolbarState InitGuiToolbar(void)
{
    GuiToolbarState state = { 0 };

    state.controlAnchor = (Vector2){ 400, 680 };
    state.constructionAnchor = (Vector2){ 0, 680 };
    state.viewAnchor = (Vector2){ 216, 0 };
    state.generalAnchor = (Vector2){ 0, 0 };
    state.settingAnchor = (Vector2){ 480, 0 };
    
    // File
    state.newFilePressed = false;
    state.savePressed = false;
    state.loadPressed = false;
    state.undoPressed = false;
    state.redoPressed = false;
    state.takeScreenshotPressed = false;

    // View
    state.showGridActive = true;
    state.snapToGridActive = true;

    // Settings
    state.openSettingPressed = false;
    state.algorithmDropdownBoxEditMode = false;
    state.algorithmDropdownBoxActive = 0;
    state.runAlgorithmButtonPressed = false;
    state.goToPreviousStepPressed = false;
    state.goToNextStepPressed = false;
    state.autoForwardActive = true;
    state.isHoldingUIActive = true;
    state.isHiddenUiDecorativeButtonPressed = false;
    state.exitAlgorithmPressed = false;

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

    // Others
    state.currentSelectedTool = 0;
    state.canUndo = false;
    state.canRedo = false;

    return state;
}

void GuiToolbar(GuiToolbarState *state)
{
    // File
    GuiGroupBox((Rectangle){ state->generalAnchor.x + 0, state->generalAnchor.y + 0, 216, 40 }, NULL);
    if(!state->canUndo) GuiDisable();
    state->undoPressed = GuiButton((Rectangle){ state->generalAnchor.x + 112, state->generalAnchor.y + 8, 24, 24 }, "#072#");
    GuiEnable();
    if(!state->canRedo) GuiDisable();
    state->redoPressed = GuiButton((Rectangle){ state->generalAnchor.x + 140, state->generalAnchor.y + 8, 24, 24 }, "#073#");
    GuiEnable();
    state->newFilePressed = GuiButton((Rectangle){ state->generalAnchor.x + 16, state->generalAnchor.y + 8, 24, 24 }, "#008#");
    state->loadPressed = GuiButton((Rectangle){ state->generalAnchor.x + 44, state->generalAnchor.y + 8, 24, 24 }, "#005#");
    state->savePressed = GuiButton((Rectangle){ state->generalAnchor.x + 72, state->generalAnchor.y + 8, 24, 24 }, "#006#");
    state->takeScreenshotPressed = GuiButton((Rectangle){ state->generalAnchor.x + 180, state->generalAnchor.y + 8, 24, 24 }, "#183#");
    
    // View
    GuiGroupBox((Rectangle){ state->viewAnchor.x + 0, state->viewAnchor.y + 0, 264, 40 }, NULL);
    GuiLabel((Rectangle){ state->viewAnchor.x + 16, state->viewAnchor.y + 8, 32, 24 }, "View:");
    GuiToggle((Rectangle){ state->viewAnchor.x + 48, state->viewAnchor.y + 8, 88, 24 }, "#097#Show Grid", &state->showGridActive);
    GuiToggle((Rectangle){ state->viewAnchor.x + 140, state->viewAnchor.y + 8, 104, 24 }, "#050#Snap To Grid", &state->snapToGridActive);
    
    // Settings
    GuiGroupBox((Rectangle){ state->settingAnchor.x + 0, state->settingAnchor.y + 0, 520, 40 }, NULL);
    state->openSettingPressed = GuiButton((Rectangle){ state->settingAnchor.x + 480, state->settingAnchor.y + 8, 24, 24 }, "#142#");
    GuiLabel((Rectangle){ state->settingAnchor.x + 16, state->settingAnchor.y + 8, 64, 24 }, "Algorithms: ");
    state->runAlgorithmButtonPressed = GuiButton((Rectangle){ state->settingAnchor.x + 284, state->settingAnchor.y + 8, 64, 24 }, "#131#Run"); 
    state->goToPreviousStepPressed = GuiButton((Rectangle){ state->settingAnchor.x + 256, state->settingAnchor.y + 8, 24, 24 }, "#129#"); 
    state->goToNextStepPressed = GuiButton((Rectangle){ state->settingAnchor.x + 352, state->settingAnchor.y + 8, 24, 24 }, "#134#"); 
    GuiToggle((Rectangle){ state->settingAnchor.x + 416, state->settingAnchor.y + 8, 40, 24 }, "Auto", &state->autoForwardActive);
    GuiToggle((Rectangle){ state->settingAnchor.x + 480, state->settingAnchor.y + 48, 24, 24 }, "#045#", &state->isHoldingUIActive);
    state->isHiddenUiDecorativeButtonPressed = GuiButton((Rectangle){ 960, 48, 24, 24 }, "#045#"); 
    state->exitAlgorithmPressed = GuiButton((Rectangle){ state->settingAnchor.x + 380, state->settingAnchor.y + 8, 24, 24 }, "#133#"); 
    if (GuiDropdownBox((Rectangle){ state->settingAnchor.x + 80, state->settingAnchor.y + 8, 168, 24 }, "Greedy Best-first Search;Prim's Algorithm; BFS", &state->algorithmDropdownBoxActive, state->algorithmDropdownBoxEditMode)) state->algorithmDropdownBoxEditMode = !state->algorithmDropdownBoxEditMode;
    
    // Construction
    GuiGroupBox((Rectangle){ state->constructionAnchor.x + 0, state->constructionAnchor.y + 0, 400, 40 }, NULL);
    GuiLabel((Rectangle){ state->constructionAnchor.x + 16, state->constructionAnchor.y + 8, 72, 24 }, "Construction:");
    if(state->currentSelectedTool == 3) GuiDisable();
    state->penPressed = GuiButton((Rectangle){ state->constructionAnchor.x + 96, state->constructionAnchor.y + 8, 56, 24 }, "#022#Pen");
    GuiEnable();
    if(state->currentSelectedTool == 4) GuiDisable();
    state->linkPressed = GuiButton((Rectangle){ state->constructionAnchor.x + 167, state->constructionAnchor.y + 8, 56, 24 }, "#034#Link");
    GuiEnable();
    if(state->currentSelectedTool == 5) GuiDisable();
    state->dragPressed = GuiButton((Rectangle){ state->constructionAnchor.x + 248, state->constructionAnchor.y + 8, 64, 24 }, "#067#Move");
    GuiEnable();
    if(state->currentSelectedTool == 6) GuiDisable();
    state->eraserPressed = GuiButton((Rectangle){ state->constructionAnchor.x + 315, state->constructionAnchor.y + 8, 72, 24 }, "#023#Eraser");
    GuiEnable();
    state->penColorPressed = GuiButton((Rectangle){ state->constructionAnchor.x + 150, state->constructionAnchor.y + 8, 12, 24 }, NULL);
    state->linkColorPressed = GuiButton((Rectangle){ state->constructionAnchor.x + 220, state->constructionAnchor.y + 8, 12, 24 }, NULL);
    
    
    // Controls
    GuiGroupBox((Rectangle){ state->controlAnchor.x + 0, state->controlAnchor.y + 0, 600, 40 }, NULL);
    GuiLabel((Rectangle){ state->controlAnchor.x + 16, state->controlAnchor.y + 8, 48, 24 }, "Control:");
    if(state->currentSelectedTool == 1) GuiDisable();
    state->selectPressed = GuiButton((Rectangle){ state->controlAnchor.x + 64, state->controlAnchor.y + 8, 72, 24 }, "#021#Select");
    GuiEnable();
    if(state->currentSelectedTool == 2) GuiDisable();
    state->movePressed = GuiButton((Rectangle){ state->controlAnchor.x + 140, state->controlAnchor.y + 8, 64, 24 }, "#019#View");
    GuiEnable();
    state->colorPanelButtonPressed = GuiButton((Rectangle){ state->controlAnchor.x + 350, state->controlAnchor.y + 8, 12, 24 }, NULL);
    state->changeSelectedColorPressed = GuiButton((Rectangle){ state->controlAnchor.x + 296, state->controlAnchor.y + 8, 56, 24 }, "#025#Dye");
    state->deleteSelectedPressed = GuiButton((Rectangle){ state->controlAnchor.x + 221, state->controlAnchor.y + 8, 72, 24 }, "#143#Delete");
    GuiLabel((Rectangle){ state->controlAnchor.x + 384, state->controlAnchor.y + 8, 50, 24 }, "Include:");
    GuiCheckBox((Rectangle){ state->controlAnchor.x + 432, state->controlAnchor.y + 8, 24, 24 }, "Node", &state->isSelectingVertexChecked);
    GuiCheckBox((Rectangle){ state->controlAnchor.x + 496, state->controlAnchor.y + 8, 24, 24 }, "Edge", &state->isSelectingEdgeChecked);
}

#endif // GUI_TOOLBAR_IMPLEMENTATION
