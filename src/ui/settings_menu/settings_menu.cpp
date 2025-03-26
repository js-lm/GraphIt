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

        case BP::NONE: default: break;
    }
}