#include "gui.h"
#include "../application.h"
#include "../actions_center/actions_center.h"
#include "../canvas/canvas.h"

#define GUI_TOOLBAR_IMPLEMENTATION
#include "gui_toolbar.h"

GuiToolbarState ui{InitGuiToolbar()};

void GUI::draw(){
    GuiToolbar(&ui);
}

void GUI::update(){
    updateKeyboardShortcuts();

    updateFile();
    updateView();
    updateSettings();
    updateConstruction();
    updateControls();
}

void switchMode(Canvas::Mode mode){
    Application::instance().canvas().switchMode(mode);
}

void undo(){ Application::instance().actionCenter().undo();}
void redo(){ Application::instance().actionCenter().redo();}

void GUI::updateKeyboardShortcuts(){
    auto key{GetKeyPressed()};
    bool hasPress{false};

    if(IsKeyDown(KEY_LEFT_CONTROL)){
        if(IsKeyDown(KEY_LEFT_SHIFT)){
            hasPress = updateControlShiftKeys(key);
        }else{
            hasPress = updateControlKeys(key);
        }
    }

    if(!hasPress) updateKeys(key);
}

bool GUI::updateKeys(int key){
    // No Modifier Keys
    switch(key){
    case KEY_P: switchMode(Canvas::Mode::PEN);    return true;
    case KEY_L: switchMode(Canvas::Mode::LINK);   return true;
    case KEY_E: switchMode(Canvas::Mode::ERASER); return true;
    default: break;
    }
    return false;
}

bool GUI::updateControlKeys(int key){
    // Control
    switch(key){
    case KEY_Z: undo(); return true;
    default: break;
    }
    return false;
}

bool GUI::updateControlShiftKeys(int key){
    // Control + Shift
    switch(key){
    case KEY_Z: redo(); return true;
    default: break;
    }
    return false;
}

void GUI::updateFile(){
    if(ui.newFilePressed){

    }

    if(ui.savePressed){

    }

    if(ui.loadPressed){

    }
    
    if(ui.undoPressed) undo();
    if(ui.redoPressed) redo();
    
    if(ui.takeScreenshotPressed){

    }
    
    if(ui.loadLuaPressed){

    }
}

void GUI::updateView(){
    // state.showGridActive = true;
    // state.snapToGridActive = true;
}

void GUI::updateSettings(){
    // state.isDirectedChecked = false;
    // state.isWeightedChecked = false;
    // state.isShowingLabelsChecked = false;

    if(ui.openSettingPressed){

    }
}

void GUI::updateConstruction(){
    if(ui.penPressed) switchMode(Canvas::Mode::PEN);
    
    if(ui.penColorPressed){

    }
    
    if(ui.linkPressed) switchMode(Canvas::Mode::LINK);
    
    if(ui.linkColorPressed){

    }
    
    if(ui.dragPressed) switchMode(Canvas::Mode::DRAG);
    if(ui.eraserPressed) switchMode(Canvas::Mode::ERASER);
}

void GUI::updateControls(){
    if(ui.selectPressed) switchMode(Canvas::Mode::SELECT);
    if(ui.movePressed) switchMode(Canvas::Mode::MOVE);
    
    if(ui.deleteSelectedPressed){

    }
    
    if(ui.changeSelectedColorPressed){

    }
    
    if(ui.colorPanelButtonPressed){

    }

    // state.isSelectingVertexChecked = false;
    // state.isSelectingEdgeChecked = false;
}