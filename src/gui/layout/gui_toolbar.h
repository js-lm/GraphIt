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

    Vector2 settingWindowAnchor;
    Vector2 graphSettingAnchor;
    Vector2 colorPreferenceAnchor;
    Vector2 vertexEdgeSettingAnchor;
    Vector2 gridSettingsAnchor;
    
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

    // Setting Windows
    bool settingsWindowActive;
    int settingListViewScrollIndex;
    int settingListViewActive;
    bool weightPrecisionEditMode;
    int weightPrecisionValue;
    bool resetLabelsPressed;
    bool isDirectedActive;
    bool isWeightedActive;
    bool isLabeledActive;
    Color colorPreferenceColorPickerValue;
    bool vSEc2Pressed;
    bool vSEcPressed;
    bool vVUc1Pressed;
    bool vVUc2Pressed;
    bool vCScPressed;
    bool VUc2Pressed;
    bool eBSc2Pressed;
    bool eBSc1Pressed;
    bool eVUc1Pressed;
    bool eCPc1Pressed;
    bool vCS2Pressed;
    bool eCPc2Pressed;
    bool preCol1Pressed;
    bool preCol2Pressed;
    bool preCol3Pressed;
    bool preCol4Pressed;
    bool preCo5Pressed;
    bool preCol6Pressed;
    bool preCol9Pressed;
    bool preCol10Pressed;
    bool preCol11Pressed;
    bool preC12Pressed;
    bool preCol7Pressed;
    bool preCol8Pressed;
    float vertexRadiusSliderValue;
    float edgeThicknessSliderValue;
    bool vertexRadiusValueBoxEditMode;
    int vertexRadiusValueBoxValue;
    bool edgeThicknessValueBoxEditMode;
    int edgeThicknessValueBoxValue;
    bool resetAllPressed;
    bool resetColorPressed;
    bool resetGridPressed;
    float cellSizeSliderValue;
    float subdivisionSizeSliderValue;
    bool cellSizeValueBoxEditMode;
    int cellSizeValueBoxValue;
    bool subdivisionSizeValueBoxEditMode;
    int subdivisionSizeValueBoxValue;
    bool resetVertexPressed;

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
    state.settingWindowAnchor = (Vector2){ 168, 96 };
    state.graphSettingAnchor = (Vector2){ 280, 144 };
    state.colorPreferenceAnchor = (Vector2){ 280, 288 };
    state.vertexEdgeSettingAnchor = (Vector2){ 688, 144 };
    state.gridSettingsAnchor = (Vector2){ 648, 288 };
    
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

    // Setting Window
    state.settingsWindowActive = true;
    state.settingListViewScrollIndex = 0;
    state.settingListViewActive = 0;
    state.weightPrecisionEditMode = false;
    state.weightPrecisionValue = 0;
    state.resetLabelsPressed = false;
    state.isDirectedActive = true;
    state.isWeightedActive = true;
    state.isLabeledActive = true;
    state.colorPreferenceColorPickerValue = (Color){ 0, 0, 0, 0 };
    state.vSEc2Pressed = false;
    state.vSEcPressed = false;
    state.vVUc1Pressed = false;
    state.vVUc2Pressed = false;
    state.vCScPressed = false;
    state.VUc2Pressed = false;
    state.eBSc2Pressed = false;
    state.eBSc1Pressed = false;
    state.eVUc1Pressed = false;
    state.eCPc1Pressed = false;
    state.vCS2Pressed = false;
    state.eCPc2Pressed = false;
    state.preCol1Pressed = false;
    state.preCol2Pressed = false;
    state.preCol3Pressed = false;
    state.preCol4Pressed = false;
    state.preCo5Pressed = false;
    state.preCol6Pressed = false;
    state.preCol9Pressed = false;
    state.preCol10Pressed = false;
    state.preCol11Pressed = false;
    state.preC12Pressed = false;
    state.preCol7Pressed = false;
    state.preCol8Pressed = false;
    state.vertexRadiusSliderValue = 0.0f;
    state.edgeThicknessSliderValue = 0.0f;
    state.vertexRadiusValueBoxEditMode = false;
    state.vertexRadiusValueBoxValue = 0;
    state.edgeThicknessValueBoxEditMode = false;
    state.edgeThicknessValueBoxValue = 0;
    state.resetAllPressed = false;
    state.resetColorPressed = false;
    state.resetGridPressed = false;
    state.cellSizeSliderValue = 0.0f;
    state.subdivisionSizeSliderValue = 0.0f;
    state.cellSizeValueBoxEditMode = false;
    state.cellSizeValueBoxValue = 0;
    state.subdivisionSizeValueBoxEditMode = false;
    state.subdivisionSizeValueBoxValue = 0;
    state.resetVertexPressed = false;

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

    // Setting Window
    if (state->settingsWindowActive)
    {
        state->settingsWindowActive = !GuiWindowBox((Rectangle){ state->settingWindowAnchor.x + 0, state->settingWindowAnchor.y + 0, 688, 480 }, "#142#Settings");
        GuiListView((Rectangle){ state->settingWindowAnchor.x + 0, state->settingWindowAnchor.y + 24, 96, 456 }, "General;Graphics;DEBUG", &state->settingListViewScrollIndex, &state->settingListViewActive);
        state->resetAllPressed = GuiButton((Rectangle){ state->settingWindowAnchor.x + 480, state->settingWindowAnchor.y + 424, 192, 32 }, "#152#Reset All Settings To Default"); 
        state->resetColorPressed = GuiButton((Rectangle){ state->settingWindowAnchor.x + 480, state->settingWindowAnchor.y + 376, 192, 32 }, "#029#Reset Colors"); 
        state->resetGridPressed = GuiButton((Rectangle){ state->settingWindowAnchor.x + 480, state->settingWindowAnchor.y + 344, 192, 32 }, "#096#Reset Grid"); 
        state->resetVertexPressed = GuiButton((Rectangle){ state->settingWindowAnchor.x + 480, state->settingWindowAnchor.y + 312, 192, 32 }, "#032#Reset Vertex/Edge"); 

        GuiGroupBox((Rectangle){ state->graphSettingAnchor.x + 0, state->graphSettingAnchor.y + 0, 392, 128 }, "Graph Settings");
        if (GuiSpinner((Rectangle){ state->graphSettingAnchor.x + 16, state->graphSettingAnchor.y + 72, 104, 24 }, NULL, &state->weightPrecisionValue, 0, 100, state->weightPrecisionEditMode)) state->weightPrecisionEditMode = !state->weightPrecisionEditMode;
        state->resetLabelsPressed = GuiButton((Rectangle){ state->graphSettingAnchor.x + 144, state->graphSettingAnchor.y + 40, 104, 24 }, "#211#Reset Labels"); 
        GuiLabel((Rectangle){ state->graphSettingAnchor.x + 144, state->graphSettingAnchor.y + 64, 72, 24 }, "#191#Warning!");
        GuiToggle((Rectangle){ state->graphSettingAnchor.x + 16, state->graphSettingAnchor.y + 16, 104, 24 }, "#210#Is Directed", &state->isDirectedActive);
        GuiToggle((Rectangle){ state->graphSettingAnchor.x + 16, state->graphSettingAnchor.y + 48, 104, 24 }, "#178#Is Weighted", &state->isWeightedActive);
        GuiLabel((Rectangle){ state->graphSettingAnchor.x + 24, state->graphSettingAnchor.y + 96, 96, 24 }, "Weight Precision");
        GuiLabel((Rectangle){ state->graphSettingAnchor.x + 144, state->graphSettingAnchor.y + 80, 248, 24 }, "This will reset the undo/redo history as well!");
        GuiToggle((Rectangle){ state->graphSettingAnchor.x + 144, state->graphSettingAnchor.y + 16, 104, 24 }, "#031#Is Labeled", &state->isLabeledActive);
        GuiGroupBox((Rectangle){ state->colorPreferenceAnchor.x + 0, state->colorPreferenceAnchor.y + 0, 352, 272 }, "Color Preferences");
        GuiColorPicker((Rectangle){ state->colorPreferenceAnchor.x + 216, state->colorPreferenceAnchor.y + 32, 96, 96 }, NULL, &state->colorPreferenceColorPickerValue);
        GuiLine((Rectangle){ state->colorPreferenceAnchor.x + 8, state->colorPreferenceAnchor.y + 120, 176, 32 }, NULL);
        state->vSEc2Pressed = GuiButton((Rectangle){ state->colorPreferenceAnchor.x + 112, state->colorPreferenceAnchor.y + 24, 24, 24 }, NULL); 
        state->vSEcPressed = GuiButton((Rectangle){ state->colorPreferenceAnchor.x + 88, state->colorPreferenceAnchor.y + 24, 24, 24 }, NULL); 
        state->vVUc1Pressed = GuiButton((Rectangle){ state->colorPreferenceAnchor.x + 120, state->colorPreferenceAnchor.y + 56, 24, 24 }, NULL); 
        state->vVUc2Pressed = GuiButton((Rectangle){ state->colorPreferenceAnchor.x + 144, state->colorPreferenceAnchor.y + 56, 24, 24 }, NULL); 
        state->vCScPressed = GuiButton((Rectangle){ state->colorPreferenceAnchor.x + 120, state->colorPreferenceAnchor.y + 88, 24, 24 }, NULL); 
        state->VUc2Pressed = GuiButton((Rectangle){ state->colorPreferenceAnchor.x + 144, state->colorPreferenceAnchor.y + 160, 24, 24 }, NULL); 
        state->eBSc2Pressed = GuiButton((Rectangle){ state->colorPreferenceAnchor.x + 128, state->colorPreferenceAnchor.y + 224, 24, 24 }, NULL); 
        state->eBSc1Pressed = GuiButton((Rectangle){ state->colorPreferenceAnchor.x + 104, state->colorPreferenceAnchor.y + 224, 24, 24 }, NULL); 
        state->eVUc1Pressed = GuiButton((Rectangle){ state->colorPreferenceAnchor.x + 120, state->colorPreferenceAnchor.y + 160, 24, 24 }, NULL); 
        state->eCPc1Pressed = GuiButton((Rectangle){ state->colorPreferenceAnchor.x + 120, state->colorPreferenceAnchor.y + 192, 24, 24 }, NULL); 
        GuiLabel((Rectangle){ state->colorPreferenceAnchor.x + 24, state->colorPreferenceAnchor.y + 24, 64, 24 }, "Start/End:");
        state->vCS2Pressed = GuiButton((Rectangle){ state->colorPreferenceAnchor.x + 144, state->colorPreferenceAnchor.y + 88, 24, 24 }, NULL); 
        GuiLabel((Rectangle){ state->colorPreferenceAnchor.x + 24, state->colorPreferenceAnchor.y + 56, 104, 24 }, "Visited/Unvisited:");
        GuiLabel((Rectangle){ state->colorPreferenceAnchor.x + 24, state->colorPreferenceAnchor.y + 88, 96, 24 }, "Current/Special:");
        GuiLabel((Rectangle){ state->colorPreferenceAnchor.x + 24, state->colorPreferenceAnchor.y + 224, 88, 24 }, "Block/Special:");
        GuiLabel((Rectangle){ state->colorPreferenceAnchor.x + 24, state->colorPreferenceAnchor.y + 160, 104, 24 }, "Visited/Unvisited:");
        GuiLabel((Rectangle){ state->colorPreferenceAnchor.x + 24, state->colorPreferenceAnchor.y + 192, 96, 24 }, "Current/Path:");
        state->eCPc2Pressed = GuiButton((Rectangle){ state->colorPreferenceAnchor.x + 144, state->colorPreferenceAnchor.y + 192, 24, 24 }, NULL); 
        GuiGroupBox((Rectangle){ state->colorPreferenceAnchor.x + 16, state->colorPreferenceAnchor.y + 16, 160, 104 }, "Vertex");
        GuiGroupBox((Rectangle){ state->colorPreferenceAnchor.x + 16, state->colorPreferenceAnchor.y + 152, 160, 104 }, "Edge");
        GuiLabel((Rectangle){ state->colorPreferenceAnchor.x + 216, state->colorPreferenceAnchor.y + 152, 120, 24 }, "Presets:");
        state->preCol1Pressed = GuiButton((Rectangle){ state->colorPreferenceAnchor.x + 216, state->colorPreferenceAnchor.y + 168, 24, 24 }, NULL); 
        state->preCol2Pressed = GuiButton((Rectangle){ state->colorPreferenceAnchor.x + 240, state->colorPreferenceAnchor.y + 168, 24, 24 }, NULL); 
        state->preCol3Pressed = GuiButton((Rectangle){ state->colorPreferenceAnchor.x + 264, state->colorPreferenceAnchor.y + 168, 24, 24 }, NULL); 
        state->preCol4Pressed = GuiButton((Rectangle){ state->colorPreferenceAnchor.x + 288, state->colorPreferenceAnchor.y + 168, 24, 24 }, NULL); 
        state->preCo5Pressed = GuiButton((Rectangle){ state->colorPreferenceAnchor.x + 216, state->colorPreferenceAnchor.y + 192, 24, 24 }, NULL); 
        state->preCol6Pressed = GuiButton((Rectangle){ state->colorPreferenceAnchor.x + 240, state->colorPreferenceAnchor.y + 192, 24, 24 }, NULL); 
        state->preCol9Pressed = GuiButton((Rectangle){ state->colorPreferenceAnchor.x + 216, state->colorPreferenceAnchor.y + 216, 24, 24 }, NULL); 
        state->preCol10Pressed = GuiButton((Rectangle){ state->colorPreferenceAnchor.x + 240, state->colorPreferenceAnchor.y + 216, 24, 24 }, NULL); 
        state->preCol11Pressed = GuiButton((Rectangle){ state->colorPreferenceAnchor.x + 264, state->colorPreferenceAnchor.y + 216, 24, 24 }, NULL); 
        state->preC12Pressed = GuiButton((Rectangle){ state->colorPreferenceAnchor.x + 288, state->colorPreferenceAnchor.y + 216, 24, 24 }, NULL); 
        state->preCol7Pressed = GuiButton((Rectangle){ state->colorPreferenceAnchor.x + 264, state->colorPreferenceAnchor.y + 192, 24, 24 }, NULL); 
        state->preCol8Pressed = GuiButton((Rectangle){ state->colorPreferenceAnchor.x + 288, state->colorPreferenceAnchor.y + 192, 24, 24 }, NULL); 
        GuiGroupBox((Rectangle){ state->vertexEdgeSettingAnchor.x + 0, state->vertexEdgeSettingAnchor.y + 0, 152, 128 }, "Vertex/Edge Settings");
        GuiLabel((Rectangle){ state->vertexEdgeSettingAnchor.x + 16, state->vertexEdgeSettingAnchor.y + 8, 120, 24 }, "Vertex Radius:");
        GuiLabel((Rectangle){ state->vertexEdgeSettingAnchor.x + 16, state->vertexEdgeSettingAnchor.y + 64, 120, 24 }, "Edge Thickness:");
        GuiSlider((Rectangle){ state->vertexEdgeSettingAnchor.x + 16, state->vertexEdgeSettingAnchor.y + 24, 120, 16 }, NULL, NULL, &state->vertexRadiusSliderValue, 0, 100);
        GuiSlider((Rectangle){ state->vertexEdgeSettingAnchor.x + 16, state->vertexEdgeSettingAnchor.y + 80, 120, 16 }, NULL, NULL, &state->edgeThicknessSliderValue, 0, 100);
        if (GuiValueBox((Rectangle){ state->vertexEdgeSettingAnchor.x + 16, state->vertexEdgeSettingAnchor.y + 40, 40, 16 }, NULL, &state->vertexRadiusValueBoxValue, 0, 100, state->vertexRadiusValueBoxEditMode)) state->vertexRadiusValueBoxEditMode = !state->vertexRadiusValueBoxEditMode;
        if (GuiValueBox((Rectangle){ state->vertexEdgeSettingAnchor.x + 16, state->vertexEdgeSettingAnchor.y + 96, 40, 16 }, NULL, &state->edgeThicknessValueBoxValue, 0, 100, state->edgeThicknessValueBoxEditMode)) state->edgeThicknessValueBoxEditMode = !state->edgeThicknessValueBoxEditMode;
        GuiGroupBox((Rectangle){ state->gridSettingsAnchor.x + 0, state->gridSettingsAnchor.y + 0, 192, 96 }, "Grid Settings");
        GuiLabel((Rectangle){ state->gridSettingsAnchor.x + 16, state->gridSettingsAnchor.y + 8, 120, 24 }, "Cell Size:");
        GuiSlider((Rectangle){ state->gridSettingsAnchor.x + 16, state->gridSettingsAnchor.y + 24, 120, 16 }, NULL, NULL, &state->cellSizeSliderValue, 0, 100);
        GuiLabel((Rectangle){ state->gridSettingsAnchor.x + 16, state->gridSettingsAnchor.y + 48, 120, 24 }, "Subdivision Size:");
        GuiSlider((Rectangle){ state->gridSettingsAnchor.x + 16, state->gridSettingsAnchor.y + 64, 120, 16 }, NULL, NULL, &state->subdivisionSizeSliderValue, 0, 100);
        if (GuiValueBox((Rectangle){ state->gridSettingsAnchor.x + 136, state->gridSettingsAnchor.y + 24, 40, 16 }, NULL, &state->cellSizeValueBoxValue, 0, 100, state->cellSizeValueBoxEditMode)) state->cellSizeValueBoxEditMode = !state->cellSizeValueBoxEditMode;
        if (GuiValueBox((Rectangle){ state->gridSettingsAnchor.x + 136, state->gridSettingsAnchor.y + 64, 40, 16 }, NULL, &state->subdivisionSizeValueBoxValue, 0, 100, state->subdivisionSizeValueBoxEditMode)) state->subdivisionSizeValueBoxEditMode = !state->subdivisionSizeValueBoxEditMode;
        if (GuiDropdownBox((Rectangle){ state->settingAnchor.x + 80, state->settingAnchor.y + 8, 168, 24 }, "Greedy Best-first Search", &state->algorithmDropdownBoxActive, state->algorithmDropdownBoxEditMode)) state->algorithmDropdownBoxEditMode = !state->algorithmDropdownBoxEditMode;
    }
}

#endif // GUI_TOOLBAR_IMPLEMENTATION
