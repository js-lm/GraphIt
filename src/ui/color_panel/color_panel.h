#pragma once

#include "../panel.h"

#include <string>

struct Rectangle;
struct Vector2;

namespace UI{

class ColorPanel : public Panel{
public:
    enum class ButtonPressed{
        NONE,

        COLOR_1,
        COLOR_2,
        COLOR_3,
        COLOR_4,
        COLOR_5,
        COLOR_6,
        COLOR_7,
        COLOR_8,
        COLOR_9,
        COLOR_10,
        COLOR_11,
        COLOR_12,

        SAVE,
        CANCEL,
    };

public:
    ColorPanel();
    ~ColorPanel() = default;

    void draw() override;
    void update() override;

    void refreshAnchors() override;

private:
    void pressButton(ButtonPressed button){ pressedButton_ = button;}
    const ButtonPressed buttonPressed() const{ return pressedButton_;}

    void handleButtonPress() override;

private: // utilities
    void save();
    void cancel();

private: // draw
    void drawPanel();

private: // anchors
    Vector2 panelAnchor_;

private:
    ButtonPressed pressedButton_{ButtonPressed::NONE};

    Color oldColor_;
    Color newColor_;
};

} // namespace UI