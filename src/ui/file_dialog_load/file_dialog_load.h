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
        refreshAnchors();      
    };
    ~FileDialogLoad() = default;

    void draw() override;
    void update() override;

    void refreshAnchors() override;

private:
    void pressButton(ButtonPressed button){ pressedButton_ = button;}
    const ButtonPressed buttonPressed() const{ return pressedButton_;}

    void handleButtonPress() override;

    void updateFileList(); 

private: // utilities
    void goBack();

    void selectFile();
    void cancel();

private: // draw
    void drawPanel();
    void drawScrollPanel();

    void drawFilesListing();

private: // anchors
    Vector2 windowAnchor_;

private:
    ButtonPressed pressedButton_{ButtonPressed::NONE};

    bool pathEditMode_{false};
    std::string pathText_;
    
    bool filenameEditMode_{false};
    std::string fileNameText_;

    bool fileTypeEditMode_{false};
    int fileTypeActive_{0};

    Rectangle scrollPanelScrollView_{0, 0, 0, 0};
    Vector2 scrollPanelScrollOffset_{0, 0};
    Vector2 scrollPanelBoundsOffset_{0, 0};
};

} // namespace UI