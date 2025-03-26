#pragma once

#include "../panel.h"

namespace UI{

class FileDialogSave : public Panel{
public:
    enum class ButtonPressed{
        NONE,

    };

public:
    FileDialogSave(){
        id_ = ID::SAVE;
        mainPanelBounds_ = {0, 0, 0, 0};
    };
    ~FileDialogSave() = default;

    void draw() override;

private:
    void pressButton(ButtonPressed button){ pressedButton_ = button;}
    const ButtonPressed buttonPressed() const{ return pressedButton_;}

    void handleButtonPress() override;

    bool updateKeys(int key) override;
    bool updateCommandKeys(int key) override;
    bool updateCommandShiftKeys(int key) override;

private: // utilities

private: // draw

private: // anchors

private:
    ButtonPressed pressedButton_{ButtonPressed::NONE};

};

} // namespace UI