#include "color_panel.h"
#include "system/application.h"

using namespace UI;

void ColorPanel::save(){
    // TODO: don't use int
    int destination{Application::getValue<Setting, int>(Setting::COLOR_PANEL_CALL_FROM)};
    Setting tool;
    switch(destination){
    case 1: tool = Setting::COLOR_DEBUG_PEN;   break;
    case 2: tool = Setting::COLOR_DEBUG_LINK;  break;
    case 3: tool = Setting::COLOR_DEBUG_DYE;   break;
    case 0: default: break;
    };
    Application::setValue<Setting, Color>(tool, newColor_);
    cancel();
}

void ColorPanel::cancel(){
    shouldClosePanel_ = true;
}