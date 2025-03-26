#pragma once

#include "../panel.h"

#include <string>

struct Rectangle;
struct Vector2;

namespace UI{

class FileDialogLoad : public Panel{
public:
    enum class ButtonPressed{
        NONE,
        BACK,
        SELECT,
        CANCEL
    };

public:
    FileDialogLoad(){
        id_ = ID::LOAD;

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
    ~FileDialogLoad() = default;

    void draw() override;

private:
    void pressButton(ButtonPressed button){ pressedButton_ = button;}
    const ButtonPressed buttonPressed() const{ return pressedButton_;}

    void handleButtonPress() override;

private: // utilities
    void goBack();

    void selectFile();
    void cancel();

private: // draw
    void drawPanel();

private: // anchors
    Vector2 windowAnchor_;

private:
    ButtonPressed pressedButton_{ButtonPressed::NONE};

    bool pathEditMode{false};
    std::string pathText;
    
    bool filenameEditMode{false};
    std::string fileNameText;

    bool fileTypeEditMode{false};
    int fileTypeActive{0};

    Rectangle scrollPanelScrollView{0, 0, 0, 0};
    Vector2 scrollPanelScrollOffset{0, 0};
    Vector2 scrollPanelBoundsOffset{0, 0};
};

} // namespace UI