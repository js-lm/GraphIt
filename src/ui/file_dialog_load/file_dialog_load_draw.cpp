#include "file_dialog_load.h"
#include "system/application.h"

#include <raylib.h>
#include <raygui.h>
#include <cstring>
#include <filesystem>
#include <iostream>

using namespace UI;

using BP = FileDialogLoad::ButtonPressed;

namespace fs = std::filesystem;

void FileDialogLoad::drawPanel(){
    if(fileTypeEditMode_) GuiLock();

    const Vector2 &anchor{windowAnchor_};

    if(GuiWindowBox({anchor.x + 0, anchor.y + 0, 480, 300}, "#005#Select a save file")){
        shouldClosePanel_ = true;
    }

    if(GuiButton({anchor.x + 424, anchor.y + 32, 48, 24}, "<")){
        pressButton(BP::BACK);
    }

    char pathCString[128]{0};
    strcpy(pathCString, pathText_.c_str());
    if(GuiTextBox({anchor.x + 8, anchor.y + 32, 408, 24}, pathCString, 128, pathEditMode_)){
        pathEditMode_ = !pathEditMode_;
    }
    pathText_ = pathCString;

    GuiLabel({anchor.x + 8, anchor.y + 232, 64, 24}, "File name:");

    char filenameCString[128]{0};
    strcpy(filenameCString, fileNameText_.c_str());
    if(GuiTextBox({anchor.x + 72, anchor.y + 232, 280, 24}, filenameCString, 128, filenameEditMode_)){
        filenameEditMode_ = !filenameEditMode_;
    }
    fileNameText_ = filenameCString;

    if(GuiButton({anchor.x + 360, anchor.y + 232, 112, 24}, "#005#Select")){
        pressButton(BP::SELECT);
    }

    if(GuiButton({anchor.x + 360, anchor.y + 264, 112, 24}, "Cancel")){
        shouldClosePanel_ = true;
    }

    GuiLabel({anchor.x + 8, anchor.y + 264, 64, 24}, "File filter:");

    if(GuiDropdownBox({anchor.x + 72, anchor.y + 264, 280, 24}, "GraphIt! (.grt);All (*.*)", &fileTypeActive_, fileTypeEditMode_)){
        fileTypeEditMode_ = !fileTypeEditMode_;
    }
    
    GuiUnlock();
}

void FileDialogLoad::drawScrollPanel(){
    const Vector2 &anchor{windowAnchor_};

    Rectangle scrollBounds{ 
        anchor.x + 8, 
        anchor.y + 64, 
        464 - scrollPanelScrollOffset_.x, 
        160 - scrollPanelScrollOffset_.y 
    };

    Rectangle contentBounds{ 
        anchor.x + 8, 
        anchor.y + 64, 
        464, 
        20
    };

    GuiScrollPanel(
        scrollBounds,
        NULL,
        contentBounds,
        &scrollPanelScrollOffset_, 
        &scrollPanelScrollView_
    );

    drawFilesListing();
}

void FileDialogLoad::drawFilesListing(){
    Rectangle &viewBounds{scrollPanelScrollView_};
    
    BeginScissorMode(viewBounds.x, viewBounds.y, viewBounds.width, viewBounds.height);

    EndScissorMode();
}