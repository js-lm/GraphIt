#include "toolbar.hpp"
#include "system/settings.hpp"

#include <raylib.h>
#include <raygui.h>
#include <iostream>

using namespace UI;

using BP = Toolbar::ButtonPressed;

void Toolbar::drawIoBar(){
    const Vector2 &anchor{ioAnchor_};

    bool canUndo{appFlags.toolbarCanUndo};
    bool canRedo{appFlags.toolbarCanRedo};
    
    GuiGroupBox({anchor.x, anchor.y, 216, 40}, nullptr);

    if(GuiButton({anchor.x + 16, anchor.y + 8, 24, 24}, "#008#")){
        pressButton(BP::NEW_FILE);
    }

    if(GuiButton({anchor.x + 44, anchor.y + 8, 24, 24}, "#001#")){
        pressButton(BP::LOAD_FILE);
    }
        
    if(GuiButton({anchor.x + 72, anchor.y + 8, 24, 24}, "#002#")){
        pressButton(BP::SAVE_FILE);
    }
        
    if(GuiButton({anchor.x + 180, anchor.y + 8, 24, 24}, "#183#")){
        pressButton(BP::SCREENSHOT);
    }

    if(!canUndo) GuiDisable();
    if(GuiButton({anchor.x + 112, anchor.y + 8, 24, 24}, "#072#")){
        pressButton(BP::UNDO);
    }
    GuiEnable();

    if(!canRedo) GuiDisable();
    if(GuiButton({anchor.x + 140, anchor.y + 8, 24, 24}, "#073#")){
        pressButton(BP::REDO);
    }
    GuiEnable();
}

void Toolbar::drawGridSettingsBar(){
    const Vector2 &anchor{gridSettingsAnchor_};

    bool &showGrid{appSettings.gridShow};
    bool &snapToGrid{appSettings.gridIsSnapToGrid};
    
    GuiGroupBox({anchor.x + 0, anchor.y + 0, 264, 40}, nullptr);
    GuiLabel({anchor.x + 16, anchor.y + 8, 32, 24}, "View:");

    GuiToggle({anchor.x + 48, anchor.y + 8, 88, 24}, "#097#Show Grid", &showGrid);
    GuiToggle({anchor.x + 140, anchor.y + 8, 104, 24}, "#050#Snap To Grid", &snapToGrid);
}

void Toolbar::drawAlgorithmBar(){
    const Vector2 &anchor{algorithmAnchor_};

    int &algorithmDropdownOption{appSettings.algorithmDropdownOption};
    bool &autoForward{appSettings.algorithmIsAutoForward};
    bool &isFocusMode{appFlags.algorithmFocusMode};
    bool &isRunning{appFlags.algorithmIsRunning};
    
    GuiGroupBox({anchor.x + 0, anchor.y + 0, 520, 40}, nullptr);
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
        if(GuiButton({anchor.x + 284, anchor.y + 8, 64, 24}, "#131#Run")){
            pressButton(BP::RUN_ALGORITHM);
        }
    }else{
        GuiToggle({anchor.x + 284, anchor.y + 8, 64, 24}, (isRunning ? "#132#Pause" : "#131#Resume"), &isRunning);
    }

    if(GuiButton({anchor.x + 380, anchor.y + 8, 24, 24}, "#133#")){
        pressButton(BP::EXIT_ALGORITHM);
    }
        
    if(GuiButton({anchor.x + 256, anchor.y + 8, 24, 24}, "#129#")){
        pressButton(BP::PREVIOUS_STEP);
    }
        
    if(GuiButton({anchor.x + 352, anchor.y + 8, 24, 24}, "#134#")){
        pressButton(BP::NEXT_STEP);
    }
        
    GuiToggle({anchor.x + 416, anchor.y + 8, 40, 24}, "Auto", &autoForward);
    
    if(GuiButton({anchor.x + 480, anchor.y + 8, 24, 24}, "#142#")){
        pressButton(BP::OPEN_SETTING_MENU);
    }
}

void Toolbar::drawConstructionBar(){
    const Vector2 &anchor{constructionAnchor_};

    int &currentSelectedTool{appSettings.toolbarCurrentSelectedTool};
    Color &penColor{appSettings.colorDebugPen};
    Color &linkColor{appSettings.colorDebugLink};
    Color selectedColor{GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_NORMAL))};

    auto drawHighlighting{
        [selectedColor](const Rectangle &rectangle){
            DrawRectangleRec({rectangle.x, rectangle.y - 4, rectangle.width, 2}, selectedColor);
            DrawRectangleRec({rectangle.x, rectangle.y + 26, rectangle.width, 2}, selectedColor);
        }
    };

    GuiGroupBox({anchor.x + 0, anchor.y + 0, 400, 40}, nullptr);
    GuiLabel({anchor.x + 16, anchor.y + 8, 72, 24}, "Construction:");
    
    if(currentSelectedTool == 3) drawHighlighting({anchor.x + 96, anchor.y + 8, 66, 2});
    if(GuiButton({anchor.x + 96, anchor.y + 8, 56, 24}, "#022#Pen")){
        pressButton(BP::DEBUG_PEN);
    }

    if(currentSelectedTool == 4) drawHighlighting({anchor.x + 167, anchor.y + 8, 66, 2});
    if(GuiButton({anchor.x + 167, anchor.y + 8, 56, 24}, "#174#Link")){
        pressButton(BP::DEBUG_LINK);
    }

    if(currentSelectedTool == 5) drawHighlighting({anchor.x + 248, anchor.y + 8, 64, 24});
    if(GuiButton({anchor.x + 248, anchor.y + 8, 64, 24}, "#067#Move")){
        pressButton(BP::DEBUG_MOVE);
    }

    if(currentSelectedTool == 6) drawHighlighting({anchor.x + 315, anchor.y + 8, 72, 24});
    if(GuiButton({anchor.x + 315, anchor.y + 8, 72, 24}, "#028#Eraser")){
        pressButton(BP::DEBUG_ERASER);
    }

    if(GuiButton({anchor.x + 150, anchor.y + 8, 12, 24}, nullptr)){
        pressButton(BP::DEBUG_CHANGE_PEN_COLOR);
    }
    DrawRectangle(anchor.x + 150, anchor.y + 8, 12, 24, penColor);
    DrawRectangleLinesEx({anchor.x + 150, anchor.y + 8, 12, 24}, 2.0f, ColorBrightness(penColor, -.3f));
        
    if(GuiButton({anchor.x + 220, anchor.y + 8, 12, 24}, nullptr)){
        pressButton(BP::DEBUG_CHANGE_LINK_COLOR);
    }
    DrawRectangle(anchor.x + 220, anchor.y + 8, 12, 24, linkColor);
    DrawRectangleLinesEx({anchor.x + 220, anchor.y + 8, 12, 24}, 2.0f, ColorBrightness(linkColor, -.3f));

}

void Toolbar::drawControlsBar(){
    const Vector2 &anchor{controlAnchor_};

    int &currentSelectedTool{appSettings.toolbarCurrentSelectedTool};
    bool &isSelectingVertex{appSettings.toolbarIsSelectingVertex};
    bool &isSelectingEdge{appSettings.toolbarIsSelectingEdge};
    Color &dyeColor{appSettings.colorDebugDye};
    Color selectedColor{GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_NORMAL))};

    auto drawHighlighting{
        [selectedColor](const Rectangle &rectangle){
            DrawRectangleRec({rectangle.x, rectangle.y - 4, rectangle.width, 2}, selectedColor);
            DrawRectangleRec({rectangle.x, rectangle.y + 26, rectangle.width, 2}, selectedColor);
        }
    };

    GuiGroupBox({anchor.x + 0, anchor.y + 0, 600, 40}, nullptr);
    GuiLabel({anchor.x + 16, anchor.y + 8, 48, 24}, "Control:");

    if(currentSelectedTool == 1) drawHighlighting({anchor.x + 64, anchor.y + 8, 72, 24});
    if(GuiButton({anchor.x + 64, anchor.y + 8, 72, 24}, "#021#Select")){
        pressButton(BP::DEBUG_SELECT);
    }

    if(currentSelectedTool == 2) drawHighlighting({anchor.x + 140, anchor.y + 8, 64, 24});
    if(GuiButton({anchor.x + 140, anchor.y + 8, 64, 24}, "#019#View")){
        pressButton(BP::DEBUG_PAN);
    }

    if(GuiButton({anchor.x + 296, anchor.y + 8, 56, 24}, "#025#Dye")){
        pressButton(BP::DEBUG_DYE_SELECTED);
    }
        
    if(GuiButton({anchor.x + 350, anchor.y + 8, 12, 24}, nullptr)){
        pressButton(BP::DEBUG_CHANGE_DYE_COLOR);
    }
    DrawRectangle(anchor.x + 350, anchor.y + 8, 12, 24, dyeColor);
    DrawRectangleLinesEx({anchor.x + 350, anchor.y + 8, 12, 24}, 2.0f, ColorBrightness(dyeColor, -.3f));
        
    if(GuiButton({anchor.x + 221, anchor.y + 8, 72, 24}, "#143#Delete")){
        pressButton(BP::DEBUG_DELETE_SELECTED);
    }

    GuiLabel({anchor.x + 384, anchor.y + 8, 50, 24}, "Include:");
    GuiCheckBox({anchor.x + 432, anchor.y + 8, 24, 24}, "Node", &isSelectingVertex);
    GuiCheckBox({anchor.x + 496, anchor.y + 8, 24, 24}, "Edge", &isSelectingEdge);
}

void Toolbar::drawHideButton(){
    GuiToggle(hideButtonAnchor_, (isHidingUi_ ? "#045#" : "#044#"), &isHidingUi_);
}
