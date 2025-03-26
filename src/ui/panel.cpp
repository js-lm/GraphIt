#include "panel.h"

#include <raylib.h>

#if defined(__APPLE__)
    const int COMMAND_KEY{KEY_LEFT_SUPER};
#else
    const int COMMAND_KEY{KEY_LEFT_CONTROL};
#endif

using namespace UI;

bool Panel::handleInput(){
    handleButtonPress();
    handleKeyboardShortcuts();

    return false;
}

void Panel::handleKeyboardShortcuts(){
    auto key{GetKeyPressed()};
    bool hasPress{false};

    if(IsKeyDown(COMMAND_KEY)){
        if(IsKeyDown(KEY_LEFT_SHIFT)){
            hasPress = updateCommandShiftKeys(key);
        }else{
            hasPress = updateCommandKeys(key);
        }
    }

    if(!hasPress) updateKeys(key);
}

bool Panel::isMousePressedInsidePanel(int button) const{
    return IsMouseButtonPressed(button)
        && isMouseInsidePanel();
}

bool Panel::isMouseInsidePanel() const{
    bool isInside{false};
    isInside = CheckCollisionPointRec(GetMousePosition(), mainPanelBounds_);

    // for(const auto &panel : subPanelBounds_){
    //     if(CheckCollisionPointRec(GetMousePosition(), panel)){
    //         return true;
    //     }
    // }

    return isInside;
}