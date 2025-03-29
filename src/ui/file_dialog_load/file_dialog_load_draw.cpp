#include "file_dialog_load.hpp"
#include "system/application.hpp"

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

    if(GuiTextBox({anchor.x + 8, anchor.y + 32, 408, 24}, pathText_, 128, pathEditMode_)){
        pathEditMode_ = !pathEditMode_;
        trySetPath(pathText_);
    }

    GuiLabel({anchor.x + 8, anchor.y + 232, 64, 24}, "File name:");

    if(GuiTextBox({anchor.x + 72, anchor.y + 232, 280, 24}, fileNameText_, 128, filenameEditMode_)){
        filenameEditMode_ = !filenameEditMode_;
        trySetFile(fileNameText_);
    }

    if(GuiButton({anchor.x + 360, anchor.y + 232, 112, 24}, "#005#Select")){
        pressButton(BP::SELECT);
    }

    if(GuiButton({anchor.x + 360, anchor.y + 264, 112, 24}, "Cancel")){
        shouldClosePanel_ = true;
    }

    GuiLabel({anchor.x + 8, anchor.y + 264, 64, 24}, "File filter:");

    if(GuiDropdownBox({anchor.x + 72, anchor.y + 264, 280, 24}, "GraphIt! (.grt);All (*.*)", &fileTypeIndex_, fileTypeEditMode_)){
        fileTypeEditMode_ = !fileTypeEditMode_;
        switch(fileTypeIndex_){
            case 0: fileExtensionFilter_ = "grt"; break;
            case 1: default: fileExtensionFilter_.clear(); break;
        }
    }
    
    GuiUnlock();
}

void FileDialogLoad::drawScrollPanel(){
    auto originalAlignment{GuiGetStyle(LISTVIEW, TEXT_ALIGNMENT)};
    auto originalBorderWidth{GuiGetStyle(LISTVIEW, LIST_ITEMS_BORDER_WIDTH)};
    auto originalHeight{GuiGetStyle(LISTVIEW, LIST_ITEMS_HEIGHT)};

    GuiSetStyle(LISTVIEW, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
    GuiSetStyle(LISTVIEW, LIST_ITEMS_BORDER_WIDTH, 0);
    GuiSetStyle(LISTVIEW, LIST_ITEMS_HEIGHT, 16);

    const Vector2 &anchor{windowAnchor_};

    int itemIndexTemp{itemIndex_.value_or(-1)};

    GuiListView(
        {anchor.x + 8, anchor.y + 64, 464, 160}, 
        listViewString_.empty() ? "#079#This folder is empty." : listViewString_.c_str(), 
        &listViewScrollIndex_, 
        &itemIndexTemp
    );

    if(itemIndexTemp != -1 && !listViewString_.empty()){
        itemIndex_ = itemIndexTemp;
        
        if(itemIndex_ 
        && itemIndex_.value() < static_cast<int>(filesList_.size())
        && fs::is_regular_file(filesList_[itemIndex_.value()])
        ){
            selectedFile_ = filesList_[itemIndex_.value()];
            synNames();
        }
    }

    // std::cout << "listViewScrollIndex_: " << listViewScrollIndex_
    //           << "\nitemIndex_" << itemIndex_ << std::endl;

    GuiSetStyle(LISTVIEW, TEXT_ALIGNMENT, originalAlignment);
    GuiSetStyle(LISTVIEW, LIST_ITEMS_BORDER_WIDTH, originalBorderWidth);
    GuiSetStyle(LISTVIEW, LIST_ITEMS_HEIGHT, originalHeight);
}