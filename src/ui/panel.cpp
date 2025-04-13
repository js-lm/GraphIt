#include "panel.hpp"
#include "system/settings.hpp"

#include <raylib.h>

#if defined(__APPLE__)
    const int COMMAND_KEY{KEY_LEFT_SUPER};
#else
    const int COMMAND_KEY{KEY_LEFT_CONTROL};
#endif

using namespace UI;

void Panel::handleInput(){
    handleButtonPress();
    if(!appFlags.algorithmFocusMode){
        handleKeyboardShortcuts();
    }
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

    return isInside;
}

bool Panel::updateKeys(int key){
    // no modifier keys
    switch(key){
    case KEY_ESCAPE: return shouldClosePanel_ = true;
    default: break;
    }
    return false;
}