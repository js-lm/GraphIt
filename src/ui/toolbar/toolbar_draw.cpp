#include "toolbar.h"
#include "system/application.h"

#include <raylib.h>
#include <raygui.h>
#include <iostream>

using namespace UI;

void Toolbar::drawIoBar(){
    Vector2 &anchor{ioAnchor_};

    bool canUndo{Application::getValue<Flag, bool>(Flag::TOOLBAR_CAN_UNDO)};
    bool canRedo{Application::getValue<Flag, bool>(Flag::TOOLBAR_CAN_REDO)};
    
    GuiGroupBox({anchor.x, anchor.y, 216, 40}, NULL);

    newfilePressed_ = GuiButton({anchor.x + 16, anchor.y + 8, 24, 24}, "#008#");
    loadfilePressed_ = GuiButton({anchor.x + 44, anchor.y + 8, 24, 24}, "#005#");
    savefilePressed_ = GuiButton({anchor.x + 72, anchor.y + 8, 24, 24}, "#006#");
    screenshotPressed_ = GuiButton({anchor.x + 180, anchor.y + 8, 24, 24}, "#183#");

    if(!canUndo) GuiDisable();
    undoPressed_ = GuiButton({anchor.x + 112, anchor.y + 8, 24, 24}, "#072#");
    GuiEnable();

    if(!canRedo) GuiDisable();
    redoPressed_ = GuiButton({anchor.x + 140, anchor.y + 8, 24, 24}, "#073#");
    GuiEnable();
}

void Toolbar::drawGridSettingsBar(){
    Vector2 &anchor{gridSettingsAnchor_};

    bool showGrid{Application::getValue<Setting, bool>(Setting::GRID_SHOW)};
    bool snapToGrid{Application::getValue<Setting, bool>(Setting::GRID_IS_SNAP_TO_GRID)};
    
    GuiGroupBox({anchor.x + 0, anchor.y + 0, 264, 40}, NULL);
    GuiLabel({anchor.x + 16, anchor.y + 8, 32, 24}, "View:");

    GuiToggle({anchor.x + 48, anchor.y + 8, 88, 24}, "#097#Show Grid", &showGrid);
    GuiToggle({anchor.x + 140, anchor.y + 8, 104, 24}, "#050#Snap To Grid", &snapToGrid);

    Application::setValue<Setting, bool>(Setting::GRID_SHOW, showGrid);
    Application::setValue<Setting, bool>(Setting::GRID_IS_SNAP_TO_GRID, snapToGrid);
}

void Toolbar::drawAlgorithmBar(){
    Vector2 &anchor{algorithmAnchor_};

    int algorithmDropdownOption{Application::getValue<Setting, int>(Setting::ALGORITHM_DROPDOWN_OPTION)};
    bool autoForward{Application::getValue<Setting, bool>(Setting::ALGORITHM_IS_AUTO_FORWARD)};
    bool isFocusMode{Application::getValue<Flag, bool>(Flag::ALGORITHM_FOCUS_MODE)};
    bool isRunning{Application::getValue<Flag, bool>(Flag::ALGORITHM_IS_RUNNING)};
    
    GuiGroupBox({anchor.x + 0, anchor.y + 0, 520, 40}, NULL);
    GuiLabel({anchor.x + 16, anchor.y + 8, 64, 24}, "Algorithms: ");
    
    if(GuiDropdownBox(
        {anchor.x + 80, anchor.y + 8, 168, 24}, 
        "Greedy Best-first Search;Prim's Algorithm; BFS", 
        &algorithmDropdownOption, 
        algorithmDropdownBoxEditMode_
    )){
        algorithmDropdownBoxEditMode_ = !algorithmDropdownBoxEditMode_;
    }

    if(!isFocusMode){
        runAlgorithmPressed_ = GuiButton({anchor.x + 284, anchor.y + 8, 64, 24}, "#131#Run");
    }else{
        GuiToggle({anchor.x + 284, anchor.y + 8, 64, 24}, (isRunning ? "#132#Pause" : "#131#Resume"), &isRunning);
    }

    exitAlgorithmPressed_ = GuiButton({anchor.x + 380, anchor.y + 8, 24, 24}, "#133#");   
    goToPreviousStepPressed_ = GuiButton({anchor.x + 256, anchor.y + 8, 24, 24}, "#129#"); 
    goToNextStepPressed_ = GuiButton({anchor.x + 352, anchor.y + 8, 24, 24}, "#134#"); 
    GuiToggle({anchor.x + 416, anchor.y + 8, 40, 24}, "Auto", &autoForward);
    openSettingMenuPressed_ = GuiButton({anchor.x + 480, anchor.y + 8, 24, 24}, "#142#");

    Application::setValue<Setting, int>(Setting::ALGORITHM_DROPDOWN_OPTION, algorithmDropdownOption);
    Application::setValue<Setting, bool>(Setting::ALGORITHM_IS_AUTO_FORWARD, autoForward);
    // Application::setValue<Flag, bool>(Flag::ALGORITHM_FOCUS_MODE, isFocusMode);
    Application::setValue<Flag, bool>(Flag::ALGORITHM_IS_RUNNING, isRunning);
}

void Toolbar::drawConstructionBar(){
    Vector2 &anchor{constructionAnchor_};

    int currentSelectedTool{Application::getValue<Setting, int>(Setting::TOOLBAR_CURRENT_SELECTED_TOOL)};

    GuiGroupBox({anchor.x + 0, anchor.y + 0, 400, 40}, NULL);
    GuiLabel({anchor.x + 16, anchor.y + 8, 72, 24}, "Construction:");

    if(currentSelectedTool == 3) GuiDisable();
    DEBUG_penPressed_ = GuiButton({anchor.x + 96, anchor.y + 8, 56, 24}, "#022#Pen");
    GuiEnable();

    if(currentSelectedTool == 4) GuiDisable();
    DEBUG_linkPressed_ = GuiButton({anchor.x + 167, anchor.y + 8, 56, 24}, "#034#Link");
    GuiEnable();

    if(currentSelectedTool == 5) GuiDisable();
    DEBUG_movePressed_ = GuiButton({anchor.x + 248, anchor.y + 8, 64, 24}, "#067#Move");
    GuiEnable();

    if(currentSelectedTool == 6) GuiDisable();
    DEBUG_eraserPressed_ = GuiButton({anchor.x + 315, anchor.y + 8, 72, 24}, "#023#Eraser");
    GuiEnable();

    DEBUG_changePenColorPressed_ = GuiButton({anchor.x + 150, anchor.y + 8, 12, 24}, NULL);
    DEBUG_changeLinkColorPressed_ = GuiButton({anchor.x + 220, anchor.y + 8, 12, 24}, NULL);
}

void Toolbar::drawControlsBar(){
    Vector2 &anchor{controlAnchor_};

    int currentSelectedTool{Application::getValue<Setting, int>(Setting::TOOLBAR_CURRENT_SELECTED_TOOL)};
    bool isSelectingVertex{Application::getValue<Setting, bool>(Setting::TOOLBAR_IS_SELECTING_VERTEX)};
    bool isSelectingEdge{Application::getValue<Setting, bool>(Setting::TOOLBAR_IS_SELECTING_EDGE)};

    GuiGroupBox({anchor.x + 0, anchor.y + 0, 600, 40}, NULL);
    GuiLabel({anchor.x + 16, anchor.y + 8, 48, 24}, "Control:");

    if(currentSelectedTool == 1) GuiDisable();
    DEBUG_selectPressed_ = GuiButton({anchor.x + 64, anchor.y + 8, 72, 24}, "#021#Select");
    GuiEnable();

    if(currentSelectedTool == 2) GuiDisable();
    DEBUG_panPressed_ = GuiButton({anchor.x + 140, anchor.y + 8, 64, 24}, "#019#View");
    GuiEnable();

    DEBUG_dyeSelectedPressed_ = GuiButton({anchor.x + 296, anchor.y + 8, 56, 24}, "#025#Dye");
    DEBUG_changeDyeColorPressed_ = GuiButton({anchor.x + 350, anchor.y + 8, 12, 24}, NULL);
    DEBUG_deleteSelectedPressed_ = GuiButton({anchor.x + 221, anchor.y + 8, 72, 24}, "#143#Delete");

    GuiLabel({anchor.x + 384, anchor.y + 8, 50, 24}, "Include:");
    GuiCheckBox({anchor.x + 432, anchor.y + 8, 24, 24}, "Node", &isSelectingVertex);
    GuiCheckBox({anchor.x + 496, anchor.y + 8, 24, 24}, "Edge", &isSelectingEdge);

    Application::setValue<Setting, bool>(Setting::TOOLBAR_IS_SELECTING_VERTEX, isSelectingVertex);
    Application::setValue<Setting, bool>(Setting::TOOLBAR_IS_SELECTING_EDGE, isSelectingEdge);
}

void Toolbar::drawHideButton(){
    Vector2 &anchor{algorithmAnchor_};
    GuiToggle({anchor.x + 480, anchor.y + 48, 24, 24}, (isHidingUi ? "#045#" : "#044#"), &isHidingUi);
}