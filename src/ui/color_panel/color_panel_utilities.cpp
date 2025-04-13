#include "color_panel.hpp"
#include "system/settings.hpp"

using namespace UI;

void ColorPanel::save(){
    switch(appStates.colorPanelCallFrom){
    case 1: appSettings.colorDebugPen = newColor_;   break;
    case 2: appSettings.colorDebugLink = newColor_;  break;
    case 3: appSettings.colorDebugDye = newColor_;   break;
    case 0: default: break;
    };
    cancel();
}

void ColorPanel::cancel(){
    shouldClosePanel_ = true;
}