#include "file_dialog_save.hpp"

#include <raylib.h>
#include <raygui.h>
#include <cstring>
#include <filesystem>
#include <iostream>

using namespace UI;

void FileDialogSave::drawPanel(){
    shouldClosePanel_ = GuiWindowBox({windowAnchor_.x + 0, windowAnchor_.y + 0, 472, 96}, "#002#Save To");

    GuiLabel({windowAnchor_.x + 16, windowAnchor_.y + 32, 48, 24}, "Save to:");

    if(GuiTextBox({windowAnchor_.x + 64, windowAnchor_.y + 32, 400, 24}, saveDirectoryText_, 128, directoryTextBoxEditMode_)){
        directoryTextBoxEditMode_ = !directoryTextBoxEditMode_;
        // tryOpenPath(saveDirectoryText_);
    }

    GuiLabel({windowAnchor_.x + 8, windowAnchor_.y + 64, 120, 24}, "File name:");

    if(GuiTextBox({windowAnchor_.x + 64, windowAnchor_.y + 64, 328, 24}, filenameTextText_, 128, filenameTextBoxEditMode_)){
        filenameTextBoxEditMode_ = !filenameTextBoxEditMode_;
    }

    clearFilenamePressed = GuiLabelButton({windowAnchor_.x + 372, windowAnchor_.y + 68, 16, 16}, "#113#");
    saveButtonPressed = GuiButton({windowAnchor_.x + 400, windowAnchor_.y + 64, 64, 24}, "#002#Save"); 
}