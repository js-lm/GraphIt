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
    Vector2 dpiScale{GetWindowScaleDPI()};
    float physicalScreenWidth{static_cast<float>(GetRenderWidth())};
    float physicalScreenHeight{static_cast<float>(GetRenderHeight())};
    float logicalDialogWidth{472.0f};
    float logicalDialogHeight{96.0f};
    
    windowAnchor_ = {
        (physicalScreenWidth - logicalDialogWidth * dpiScale.x) / (2.0f * dpiScale.x),
        (physicalScreenHeight - logicalDialogHeight * dpiScale.y) / (2.0f * dpiScale.y)
    };
 
    mainPanelBounds_ = {
        windowAnchor_.x, 
        windowAnchor_.y,
        472.0f,
        96.0f
    };
}