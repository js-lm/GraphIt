#include "file_dialog_load.h"
#include "system/application.h"

#include <raylib.h>
#include <raygui.h>
#include <cstring>

using namespace UI;

using BP = FileDialogLoad::ButtonPressed;

void FileDialogLoad::drawPanel(){
    if(fileTypeEditMode) GuiLock();

    shouldExit_ = GuiWindowBox({windowAnchor_.x + 0, windowAnchor_.y + 0, 480, 300}, "#005#Select a save file");

    if(GuiButton({windowAnchor_.x + 424, windowAnchor_.y + 32, 48, 24}, "<")){
        pressButton(BP::BACK);
    }

    char pathCString[128]{0};
    strcpy(pathCString, pathText.c_str());
    if(GuiTextBox({windowAnchor_.x + 8, windowAnchor_.y + 32, 408, 24}, pathCString, 128, pathEditMode)){
        pathEditMode = !pathEditMode;
    }
    pathText = pathCString;

    GuiLabel({windowAnchor_.x + 8, windowAnchor_.y + 232, 64, 24}, "File name:");

    char filenameCString[128]{0};
    strcpy(filenameCString, fileNameText.c_str());
    if(GuiTextBox({windowAnchor_.x + 72, windowAnchor_.y + 232, 280, 24}, filenameCString, 128, filenameEditMode)){
        filenameEditMode = !filenameEditMode;
    }
    fileNameText = filenameCString;

    if(GuiButton({windowAnchor_.x + 360, windowAnchor_.y + 232, 112, 24}, "#005#Select")){
        pressButton(BP::SELECT);
    }

    if(GuiButton({windowAnchor_.x + 360, windowAnchor_.y + 264, 112, 24}, "Cancel")){
        shouldExit_ = true;
    }

    GuiLabel({windowAnchor_.x + 8, windowAnchor_.y + 264, 64, 24}, "File filter:");

    if(GuiDropdownBox({windowAnchor_.x + 72, windowAnchor_.y + 264, 280, 24}, "GraphIt! (.grt);All (*.*)", &fileTypeActive, fileTypeEditMode)){
        fileTypeEditMode = !fileTypeEditMode;
    }
    
    GuiUnlock();
}