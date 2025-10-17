#include "color_panel.hpp"
#include "system/settings.hpp"

using namespace UI;

ColorPanel::ColorPanel(){
    id_ = ID::COLOR;

    shouldBlueBackground_ = true;

    refreshAnchors();

    oldColor_ = newColor_ = appSettings.colorDebugPen;
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

void ColorPanel::refreshAnchors(){
    Vector2 dpiScale{GetWindowScaleDPI()};
    float physicalScreenWidth{static_cast<float>(GetRenderWidth())};
    float physicalScreenHeight{static_cast<float>(GetRenderHeight())};
    float logicalWidth{336.0f};
    float logicalHeight{208.0f};
    
    panelAnchor_ = {
        (physicalScreenWidth - logicalWidth * dpiScale.x) / (2.0f * dpiScale.x), 
        (physicalScreenHeight - logicalHeight * dpiScale.y) / (2.0f * dpiScale.y)
    };
    
    mainPanelBounds_ = {
        panelAnchor_.x,
        panelAnchor_.y,
        336,
        208
    };
}