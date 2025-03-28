#include "file_dialog_load.h"
#include "system/application.h"

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
            doubleLeftClickCD_ = .5f;
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