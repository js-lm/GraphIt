#include "gui.h"
#include "system/application.h"
#include "canvas/canvas.h"
#include "io/io.h"
#include "magic_enum.hpp"
#include "system/terminal_prefix.h"

#define GUI_COLORPANEL_IMPLEMENTATION
#include "layout/gui_color_panel.h"

#define GUI_LOAD_FROM_IMPLEMENTATION
#include "layout/gui_load_from.h"

#include <iostream>
#include <algorithm>

namespace fs = std::filesystem;

GuiColorPanelState colorPanelGUI{InitGuiColorPanel()};
GuiLoadFromState loadFromGUI{InitGuiLoadFrom()};

bool GUI::isMouseInsidePanel() const{
    switch(panel_){
    case Panel::SAVE_TO:
    case Panel::LOAD_FROM:
    case Panel::RESET_CONFIRM:
        return true;
    case Panel::COLOR_PANEL_PEN:
        return CheckCollisionPointRec(
            GetMousePosition(),
            {150, 525, 136, 152}
        );
    case Panel::COLOR_PANEL_LINK:
        return CheckCollisionPointRec(
            GetMousePosition(),
            {220, 525, 136, 152}
        );
    case Panel::COLOR_PANEL_DYE:
        return CheckCollisionPointRec(
            GetMousePosition(),
            {750, 525, 136, 152}
        );
    case Panel::NONE: default: return false;
    }
}

void GUI::closePanel(){
    loadFromGUI.loadFileDialogActive = false;
    colorPanelGUI.colorPanelWindowActive = false;
    panel_ = Panel::NONE;
}

void GUI::switchPanel(Panel panel){
    panel_ = panel;

    switch(panel_){
    case Panel::SAVE_TO:

        break;
    case Panel::LOAD_FROM:
        loadFromGUI.loadFileDialogActive = true;
        break;
    case Panel::RESET_CONFIRM:
        
        break;
    case Panel::COLOR_PANEL_PEN:
    case Panel::COLOR_PANEL_LINK:
    case Panel::COLOR_PANEL_DYE:
        colorPanelGUI.colorPanelWindowActive = true;
        break;
    case Panel::NONE: default: break;
    }

    // std::cout << magic_enum::enum_name(panel_) << " "
    //           << loadFromGUI.loadFileDialogActive << std::endl;
}

void GUI::drawPanels(){
    if(!colorPanelGUI.colorPanelWindowActive
    && !loadFromGUI.loadFileDialogActive
    ){
        panel_ = Panel::NONE;
    }

    auto &penColor{Application::instance().canvas().getPenColorReference()};
    auto &linkColor{Application::instance().canvas().getLinkColorReference()};
    auto &dyeColor{Application::instance().canvas().getDyeColorReference()};

    drawColorTips(penColor, linkColor, dyeColor);

    switch(panel_){
    case Panel::SAVE_TO:

        break;
    case Panel::LOAD_FROM:          drawLoadPanel(); break;
    case Panel::RESET_CONFIRM:
        
        break;
    case Panel::COLOR_PANEL_PEN:    drawPenColorPanel(penColor); break;
    case Panel::COLOR_PANEL_LINK:   drawLinkColorPanel(linkColor); break;
    case Panel::COLOR_PANEL_DYE:    drawDyeColorPanel(dyeColor); break;
    case Panel::NONE: default: break;
    }
}

void GUI::drawLoadPanel(){
    DrawGuiLoadPanel(&loadFromGUI);
    // DrawGuiLoadScrollPanel(&loadFromGUI);

    drawLoadScrollPanel();
}

void GUI::drawLoadScrollPanel(){
    if(!loadFromGUI.loadFileDialogActive) return;

    Rectangle scrollBounds{ 
        loadFromGUI.anchor.x + 8, 
        loadFromGUI.anchor.y + 64, 
        464 - loadFromGUI.ScrollPanelBoundsOffset.x, 
        160 - loadFromGUI.ScrollPanelBoundsOffset.y 
    };

    Rectangle contentBounds{ 
        loadFromGUI.anchor.x + 8, 
        loadFromGUI.anchor.y + 64, 
        464, 
        20 // 160 
    };

    Rectangle &viewBounds{loadFromGUI.ScrollPanelScrollView};

    std::vector<fs::path> files;
    
    fs::path currentPath{loadDirectory_.path()};
    
    // check the path exist first
    if(fs::exists(currentPath) && fs::is_directory(currentPath)){
        try{
            for(const auto &entry : fs::directory_iterator(currentPath)){
                fs::path filePath{entry.path()};
                files.emplace_back(filePath);
            }
        }catch(const fs::filesystem_error &error){
            std::cerr << "Unable to read the directory: " << error.what() << std::endl;
        }
    }else{
        std::cerr << "Not a valid directory: " << currentPath.string() << std::endl;
        resetLoadDirectory();
        return;
    }

    std::sort(
        files.begin(), 
        files.end(), 
        [](const fs::path &directoryA, const fs::path &directoryB){
            bool isADirectory{fs::is_directory(directoryA)};
            bool isBDirectory{fs::is_directory(directoryB)};
            
            if(isADirectory != isBDirectory){
                return isADirectory;
            }
            return directoryA < directoryB;
        }
    );

    const int entryLineHeight{20};
    contentBounds.height += files.size() * entryLineHeight;

    GuiScrollPanel(
        scrollBounds,
        NULL,
        contentBounds,
        &loadFromGUI.ScrollPanelScrollOffset, 
        &viewBounds
    );
    
    BeginScissorMode(viewBounds.x, viewBounds.y, viewBounds.width, viewBounds.height);

    for (size_t i{0}; i < files.size(); i++) {
        Rectangle fileRectangle{
            viewBounds.x + 10, 
            viewBounds.y + i * entryLineHeight + loadFromGUI.ScrollPanelScrollOffset.y, 
            464, 
            entryLineHeight
        };

        std::string filename{files[i].filename().string()};
        std::string displayName;

        if(fs::is_regular_file(files[i])){
            displayName = "#218# " + filename;
        }else if(fs::is_directory(files[i])) {
            displayName = "#217# " + filename;
        }

        if(GuiLabelButton(fileRectangle, displayName.c_str())){
            if(fs::is_regular_file(files[i])){
                // loadFilename_ = filename;
                strcpy(loadFromGUI.fileNameText, filename.c_str());
            }else if(fs::is_directory(files[i])) {
                // TODO: I need a help function to update the path + text
                loadDirectory_ = fs::directory_entry(files[i]);
                strcpy(loadFromGUI.pathText, loadDirectory_.path().string().c_str());
                loadFilename_ = "";
            }
        }
    }

    EndScissorMode();
}

void GUI::drawColorTips(const Color &pen, const Color &link, const Color &dye) const{
    DrawRectangle(150, 688, 12, 24, pen);
    DrawRectangleLinesEx({150, 688, 12, 24}, 2.0f, ColorBrightness(pen, -.3f));
    DrawRectangle(220, 688, 12, 24, link);
    DrawRectangleLinesEx({220, 688, 12, 24}, 2.0f, ColorBrightness(link, -.3f));
    DrawRectangle(750, 688, 12, 24, dye);
    DrawRectangleLinesEx({750, 688, 12, 24}, 2.0f, ColorBrightness(dye, -.3f));
}

void GUI::drawPenColorPanel(Color &color){
    DrawGuiColorPanel(
        &colorPanelGUI, 
        {150, 525}, 
        &color,
        "Vertex Color"
    );
}

void GUI::drawLinkColorPanel(Color &color){
    DrawGuiColorPanel(
        &colorPanelGUI, 
        {220, 525}, 
        &color,
        "Edge Color"
    );
}

void GUI::drawDyeColorPanel(Color &color){
    DrawGuiColorPanel(
        &colorPanelGUI, 
        {750, 525}, 
        &color,
        "Dye Color"
    );
}

void GUI::updateLoadPanel(){
    if(loadFromGUI.fileCancelPressed) handleLoadFileGuiCancelPressed();
    if(loadFromGUI.fileSelectPressed) handleLoadFileButtonPressed();
    if(loadFromGUI.directoryBackPressed) handleLoadFileDirectoryGoBack();
}

void GUI::handleLoadFileDirectoryGoBack(){
    if(loadDirectory_.path().has_parent_path()){
        loadDirectory_ = fs::directory_entry(loadDirectory_.path().parent_path());
        strcpy(loadFromGUI.pathText, loadDirectory_.path().string().c_str());
    }
}

void GUI::handleLoadFileButtonPressed(){
    loadDirectory_ = fs::directory_entry(fs::path(loadFromGUI.pathText));
    loadFilename_ = loadFromGUI.fileNameText;

    checkLoadDirectory();
    if(checkLoadFile()){
        printGuiPrefix();
        std::cout << "Opening: \"" << loadDirectory_ 
                  << "\" \"" << loadFilename_ << "\"" << std::endl;
        Application::instance().serializer().load(getFullLoadPath());
    }else{
        printErrorPrefix();
        std::cerr << "Failed to check load file \"" << loadDirectory_ 
                  << "\" \"" << loadFilename_ << "\"" << std::endl;
    }
}

void GUI::handleLoadFileGuiCancelPressed(){
    closePanel();
    loadFromGUI.fileCancelPressed = false;
}

bool GUI::checkLoadFile(){
    fs::path file{getFullLoadPath()};
    if(fs::exists(file) && fs::is_regular_file(file)){
        return true;
    }
    return false;
}

void GUI::checkLoadDirectory(){
    if(!fs::exists(loadDirectory_)
    || !fs::is_directory(loadDirectory_)
    ){
        resetLoadDirectory();
    }
}

void GUI::resetLoadDirectory(){
    loadDirectory_ = std::filesystem::directory_entry(std::filesystem::current_path());
    strcpy(loadFromGUI.pathText, loadDirectory_.path().string().c_str());
}