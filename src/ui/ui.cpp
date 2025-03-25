#include "ui.h"
#include "toolbar/toolbar.h"

UI::Center::Center(){
    openedPanels_.push_back(std::make_unique<UI::Toolbar>());
}

void UI::Center::draw() {
    for(auto &panel : openedPanels_){
        panel->draw();
    }
}

void UI::Center::update(){
    for(auto &panel : openedPanels_){
        panel->update();
    }
    openedPanels_.back()->handleInput();
}