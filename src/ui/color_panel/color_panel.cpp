#include "color_panel.h"
#include "system/application.h"

using namespace UI;

ColorPanel::ColorPanel()
    : panelAnchor_({
        (float)(GetScreenWidth() - 336) / 2.0f, 
        (float)(GetScreenHeight() - 208) / 2.0f
    })
{
    id_ = ID::COLOR;

    shouldBlueBackground_ = true;

    mainPanelBounds_ = {
        panelAnchor_.x,
        panelAnchor_.y,
        336,
        208
    };

    // TODO: don't use int
    int destination{Application::getValue<Setting, int>(Setting::COLOR_PANEL_CALL_FROM)};
    Setting tool;
    switch(destination){
    case 1: tool = Setting::COLOR_DEBUG_PEN;   break;
    case 2: tool = Setting::COLOR_DEBUG_LINK;  break;
    case 3: tool = Setting::COLOR_DEBUG_DYE;   break;
    case 0: default: break;
    };
    Color color{Application::getValue<Setting, Color>(tool)};

    oldColor_ = newColor_ = color;
};

void ColorPanel::draw(){
    pressButton(ButtonPressed::NONE);
    drawPanel();
}

void ColorPanel::update(){
    // if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
    // && !isMousePressedInsidePanel(MOUSE_BUTTON_LEFT)
    // ){
    //     cancel();
    // }
}

void ColorPanel::handleButtonPress(){
    using BP = ButtonPressed;

    switch(buttonPressed()){
    case BP::SAVE: save(); break;
    case BP::CANCEL: cancel(); break;
        
    case BP::NONE: default: break;
    }
}