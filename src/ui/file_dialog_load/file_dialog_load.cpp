#include "file_dialog_load.h"
#include "system/application.h"

using namespace UI;

void FileDialogLoad::draw(){
    pressButton(ButtonPressed::NONE);
    drawPanel();
    drawScrollPanel();
}

void FileDialogLoad::update(){
    // updateFileList();
}

void FileDialogLoad::handleButtonPress(){
    using BP = ButtonPressed;

    switch(buttonPressed()){
    case BP::BACK:      goBack();       break;

    case BP::SELECT:    selectFile();   break;
    case BP::CANCEL:    cancel();       break;
        
    case BP::NONE: default: break;
    }
}

void FileDialogLoad::updateFileList(){

}

void FileDialogLoad::refreshAnchors(){
    Vector2 anchor{
        (GetScreenWidth() - 480) / 2.0f, 
        (GetScreenHeight() - 300) / 2.0f
    };
    
    windowAnchor_ = anchor;
 
    mainPanelBounds_ = {
        anchor.x, 
        anchor.y,
        480.0f,
        300.0f
    };
}