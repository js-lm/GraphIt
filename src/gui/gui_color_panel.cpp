#include "gui.h"
#include "../application.h"
#include "../canvas/canvas.h"

#define GUI_COLORPANEL_IMPLEMENTATION
#include "layout/gui_color_panel.h"

GuiColorPanelState colorPanelGUI{InitGuiColorPanel()};

bool GUI::isMouseInsidePanel() const{
    switch(colorPanelMode_){
    case ColorPanelMode::PEN:
        return CheckCollisionPointRec(
            GetMousePosition(),
            {150, 525, 136, 152}
        );
    case ColorPanelMode::LINK:
        return CheckCollisionPointRec(
            GetMousePosition(),
            {220, 525, 136, 152}
        );
    case ColorPanelMode::DYE:
        return CheckCollisionPointRec(
            GetMousePosition(),
            {750, 525, 136, 152}
        );
    case ColorPanelMode::NONE: default: return false;
    }
}

void GUI::switchColorPanel(ColorPanelMode mode){
    colorPanelMode_ = mode;
    if(mode != ColorPanelMode::NONE){
        colorPanelGUI.colorPanelWindowActive = true;
    }
}

void GUI::drawColorPanel(){
    if(!colorPanelGUI.colorPanelWindowActive){
        colorPanelMode_ = ColorPanelMode::NONE;
    }

    auto &penColor{Application::instance().canvas().getPenColorReference()};
    auto &linkColor{Application::instance().canvas().getLinkColorReference()};
    auto &dyeColor{Application::instance().canvas().getDyeColorReference()};

    // colored buttons
    DrawRectangle(150, 688, 12, 24, penColor);
    DrawRectangleLinesEx({150, 688, 12, 24}, 2.0f, ColorBrightness(penColor, -.3f));
    DrawRectangle(220, 688, 12, 24, linkColor);
    DrawRectangleLinesEx({220, 688, 12, 24}, 2.0f, ColorBrightness(linkColor, -.3f));
    DrawRectangle(750, 688, 12, 24, dyeColor);
    DrawRectangleLinesEx({750, 688, 12, 24}, 2.0f, ColorBrightness(dyeColor, -.3f));

    switch(colorPanelMode_){
    case ColorPanelMode::PEN:
        DrawGuiColorPanel(
            &colorPanelGUI, 
            {150, 525}, 
            &penColor,
            "Vertex Color"
        );
        break;
    case ColorPanelMode::LINK:
        DrawGuiColorPanel(
            &colorPanelGUI, 
            {220, 525}, 
            &linkColor,
            "Edge Color"
        );
        break;
    case ColorPanelMode::DYE:
        DrawGuiColorPanel(
            &colorPanelGUI, 
            {750, 525}, 
            &dyeColor,
            "Dye Color"
        );
        break;
    case ColorPanelMode::NONE: default: return;
    }
}