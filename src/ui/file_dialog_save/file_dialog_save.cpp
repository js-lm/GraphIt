#include "file_dialog_save.hpp"

#include <raylib.h>
#include <iostream>

using namespace UI;

void FileDialogSave::draw(){
    drawPanel();
}

void FileDialogSave::handleButtonPress(){
    if(saveButtonPressed) save();
    if(clearFilenamePressed) setFilename();
}

void FileDialogSave::refreshAnchors(){
    windowAnchor_ = {
        (GetScreenWidth() - 472) / 2.0f, 
        (GetScreenHeight() - 96) / 2.0f
    };
 
    mainPanelBounds_ = {
        windowAnchor_.x, 
        windowAnchor_.y,
        472.0f,
        96.0f
    };
}