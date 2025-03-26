#pragma once

#include "../panel.h"

#include <string>

struct Rectangle;
struct Vector2;

namespace UI{

class SettingsWindow : public Panel{
public:
    enum class ButtonPressed{
        NONE,

    };

public:
SettingsWindow(){
        id_ = ID::SETTINGS;

        shouldBlueBackground_ = true;

        windowAnchor_ ={
            (float)(GetScreenWidth() - 480) / 2.0f, 
            (float)(GetScreenHeight() - 300) / 2.0f
        };

        mainPanelBounds_ = {
            windowAnchor_.x, 
            windowAnchor_.y,
            480.0f,
            300.0f
        };
        
    };
    ~SettingsWindow() = default;

    void draw() override;

private:
    void pressButton(ButtonPressed button){ pressedButton_ = button;}
    const ButtonPressed buttonPressed() const{ return pressedButton_;}

    void handleButtonPress() override;

private: // utilities


private: // draw


private: // anchors
    

private:
    ButtonPressed pressedButton_{ButtonPressed::NONE};


};

} // namespace UI