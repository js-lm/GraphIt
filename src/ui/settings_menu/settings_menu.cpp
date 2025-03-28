#include "settings_menu.h"
#include "system/application.h"

using namespace UI;

void SettingsMenu::draw(){
    pressButton(ButtonPressed::NONE);
    drawMainWindow();

    // general
    drawGeneralGraphSettings();
    drawGeneralColorPreferences();
    drawGeneralVertexEdgeSettings();
    drawGeneralGridSetting();
}

void SettingsMenu::handleButtonPress(){
    using BP = ButtonPressed;

    switch(buttonPressed()){
        // graph settings
        case BP::GRAPH_RESET_LABEL: resetLabel(); break;

        // vertex/edge settings
        case BP::VISUAL_RESET_SIZE: resetVertexEdgeSize(); break;

        // color preferences
        case BP::COLOR_RESET:           selectButton(BP::COLOR_RESET); resetColor();    break;
        case BP::COLOR_VERTEX_START:    selectButton(BP::COLOR_VERTEX_START);           break;
        case BP::COLOR_VERTEX_END:      selectButton(BP::COLOR_VERTEX_END);             break;
        case BP::COLOR_VERTEX_VISTED:   selectButton(BP::COLOR_VERTEX_VISTED);          break;
        case BP::COLOR_VERTEX_UNVISITED: selectButton(BP::COLOR_VERTEX_UNVISITED);      break;
        case BP::COLOR_VERTEX_CURRENT:  selectButton(BP::COLOR_VERTEX_CURRENT);         break;
        case BP::COLOR_VERTEX_SPECIAL:  selectButton(BP::COLOR_VERTEX_SPECIAL);         break;
        case BP::COLOR_EDGE_VISTED:     selectButton(BP::COLOR_EDGE_VISTED);            break;
        case BP::COLOR_EDGE_UNVISITED:  selectButton(BP::COLOR_EDGE_UNVISITED);         break;
        case BP::COLOR_EDGE_CURRENT:    selectButton(BP::COLOR_EDGE_CURRENT);           break;
        case BP::COLOR_EDGE_PATH:       selectButton(BP::COLOR_EDGE_PATH);              break;
        case BP::COLOR_EDGE_BLOCK:      selectButton(BP::COLOR_EDGE_BLOCK);             break;
        case BP::COLOR_EDGE_SPECIAL:    selectButton(BP::COLOR_EDGE_SPECIAL);           break;

        case BP::COLOR_PRESET_1:    changeSelectedButtonColor(BP::COLOR_PRESET_1);  break;
        case BP::COLOR_PRESET_2:    changeSelectedButtonColor(BP::COLOR_PRESET_2);  break;
        case BP::COLOR_PRESET_3:    changeSelectedButtonColor(BP::COLOR_PRESET_3);  break;
        case BP::COLOR_PRESET_4:    changeSelectedButtonColor(BP::COLOR_PRESET_4);  break;
        case BP::COLOR_PRESET_5:    changeSelectedButtonColor(BP::COLOR_PRESET_5);  break;
        case BP::COLOR_PRESET_6:    changeSelectedButtonColor(BP::COLOR_PRESET_6);  break;
        case BP::COLOR_PRESET_7:    changeSelectedButtonColor(BP::COLOR_PRESET_7);  break;
        case BP::COLOR_PRESET_8:    changeSelectedButtonColor(BP::COLOR_PRESET_8);  break;
        case BP::COLOR_PRESET_9:    changeSelectedButtonColor(BP::COLOR_PRESET_9);  break;
        case BP::COLOR_PRESET_10:   changeSelectedButtonColor(BP::COLOR_PRESET_10); break;
        case BP::COLOR_PRESET_11:   changeSelectedButtonColor(BP::COLOR_PRESET_11); break;
        case BP::COLOR_PRESET_12:   changeSelectedButtonColor(BP::COLOR_PRESET_12); break;
        
        // grid setting
        case BP::GRID_RESET: resetGrid(); break;

        // reset all
        case BP::RESET_ALL: resetALL(); break;

        case BP::NONE: default: break;
    }
}

void SettingsMenu::refreshAnchors(){
    Vector2 anchor{
        (GetScreenWidth() - 688) / 2.0f, 
        (GetScreenHeight() - 480) / 2.0f
    };
    
    settingWindowAnchor_ = anchor;
 
    mainPanelBounds_ = {
        anchor.x, 
        anchor.y,
        688.0f,
        480.0f
    };

    // general
    graphSettingAnchor_ = {anchor. x + 112, anchor. y + 48};
    colorPreferencesAnchor_ = {anchor. x + 112, anchor. y + 192};
    vertexEdgeSettingsAnchor_ = {anchor. x + 520, anchor. y + 48};
    gridSettingsAnchor_ = {anchor. x + 480, anchor. y + 192};
}