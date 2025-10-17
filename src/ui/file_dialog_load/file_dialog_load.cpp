#include "file_dialog_load.hpp"

#include <raylib.h>
#include <iostream>

using namespace UI;

void FileDialogLoad::draw(){
    pressButton(ButtonPressed::NONE);
    drawPanel();
    drawScrollPanel();
}

void FileDialogLoad::update(){
    updateFileList();
    fetchUserInput();
}

void FileDialogLoad::handleButtonPress(){
    using BP = ButtonPressed;

    switch(buttonPressed()){
    case BP::BACK:      goBack();       break;

    case BP::SELECT:    loadSelectedFile();   break;
    case BP::CANCEL:    cancel();       break;
        
    case BP::NONE: default: break;
    }
}

void FileDialogLoad::fetchUserInput(){
    if(CheckCollisionPointRec(
        GetMousePosition(), 
        {windowAnchor_.x + 8, windowAnchor_.y + 64, 464, 160})
    && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
    ){
        if(doubleLeftClickCD_){
            openSelectedFile();
            doubleLeftClickCD_ = std::nullopt;
        }else{
            doubleLeftClickCD_ = .3f;
        }
    }
    
    if(doubleLeftClickCD_){
        *doubleLeftClickCD_ -= GetFrameTime();
        if(doubleLeftClickCD_.value() <= 0){
            doubleLeftClickCD_ = std::nullopt;
        }
    }

    if(!(pathEditMode_ && filenameEditMode_ && fileTypeEditMode_)){
        if(IsKeyPressed(KEY_ENTER)) openSelectedFile();
    }
}

void FileDialogLoad::refreshAnchors(){
    Vector2 dpiScale{GetWindowScaleDPI()};
    float physicalScreenWidth{static_cast<float>(GetRenderWidth())};
    float physicalScreenHeight{static_cast<float>(GetRenderHeight())};
    float logicalDialogWidth{480.0f};
    float logicalDialogHeight{300.0f};
    
    Vector2 anchor{
        (physicalScreenWidth - logicalDialogWidth * dpiScale.x) / (2.0f * dpiScale.x),
        (physicalScreenHeight - logicalDialogHeight * dpiScale.y) / (2.0f * dpiScale.y)
    };
    
    windowAnchor_ = anchor;
 
    mainPanelBounds_ = {
        anchor.x, 
        anchor.y,
        480.0f,
        300.0f
    };
}