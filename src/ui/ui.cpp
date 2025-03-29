#include "ui.hpp"
#include "toolbar/toolbar.hpp"
#include "file_dialog_load/file_dialog_load.hpp"
#include "settings_menu/settings_menu.hpp"
#include "color_panel/color_panel.hpp"
#include "file_dialog_save/file_dialog_save.hpp"

#include <raylib.h>
#include <raygui.h>

using namespace UI;

Center::Center(){
    openedPanels_.push_back(std::make_unique<UI::Toolbar>());
}

void Center::draw(){
    for(auto &panel : openedPanels_){
        if(panel != openedPanels_.back()){
            GuiLock();
        }
        if(panel->shouldBlurBackground()){
            DrawRectangle(
                0,
                0,
                GetScreenWidth(),
                GetScreenHeight(),
                Fade(BLACK, .5f)
            );
        }
        panel->draw();
        GuiUnlock();
    }
}

void Center::update(){
    for(auto i{openedPanels_.size()}; i--> 0;){
        openedPanels_[i]->update();
        if(openedPanels_[i]->shouldClosePanel()){
            openedPanels_.erase(openedPanels_.begin() + i);
        }
    }
    openedPanels_.back()->handleInput();
}

bool Center::isMouseOnCanvas() const{
    if(openedPanels_.size() > 1) return false;

    return !openedPanels_.empty() 
        && !openedPanels_.back()->isMouseInsidePanel();
}

void Center::updatePanelAnchors(){
    for(const auto &panel : openedPanels_){
        panel->refreshAnchors();
    }
}

void Center::openPanel(Panel::ID id){
    switch(id){
    case Panel::ID::TOOLBAR:
        openedPanels_.push_back(std::make_unique<UI::Toolbar>());
        break;
    case Panel::ID::SAVE:
        openedPanels_.push_back(std::make_unique<UI::FileDialogSave>());
        break;
    case Panel::ID::LOAD:
        openedPanels_.push_back(std::make_unique<UI::FileDialogLoad>());
        break;
    case Panel::ID::SETTINGS:
        openedPanels_.push_back(std::make_unique<UI::SettingsMenu>());
        break;
    case Panel::ID::COLOR:
        openedPanels_.push_back(std::make_unique<UI::ColorPanel>());
        break;
    
    default: break;
    }
}

void Center::closeTopPanel(){
    if(openedPanels_.size() > 1){
        openedPanels_.pop_back();
    }
}