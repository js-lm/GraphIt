#include "toolbar.hpp"
#include "system/application.hpp"

#include <raygui.h>

using namespace UI;

void Toolbar::draw(){
    if(algorithmDropdownBoxEditMode_) GuiLock();

    if(!isHidingUi_) DrawRectangleRec(mainPanelBounds_, Fade(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)), .5f));

    drawHideButton();
    if(isHidingUi_) return;

    pressButton(ButtonPressed::NONE);

    drawConstructionBar();
    drawControlsBar();
    drawIoBar();
    drawGridSettingsBar();
    drawAlgorithmBar();

    if(algorithmDropdownBoxEditMode_) GuiUnlock();
}

void Toolbar::handleButtonPress(){
    using BP = ButtonPressed;

    switch(buttonPressed()){
    // setting
    case BP::OPEN_SETTING_MENU:     openSettingMenu();  break;
        
    // IO bar
    case BP::NEW_FILE:      createNewFile();        break;
    case BP::LOAD_FILE:     loadSavedGraph();       break;
    case BP::SAVE_FILE:     saveCurrentGraph();     break;
    case BP::UNDO:          undo();                 break;
    case BP::REDO:          redo();                 break;
    case BP::SCREENSHOT:    takeScreenshot();       break;
        
    // algorithm bar
    case BP::RUN_ALGORITHM:     enterAlgorithmMode();       break;
    case BP::EXIT_ALGORITHM:    exitAlgorithmMode();        break;
    case BP::PREVIOUS_STEP:     goToPreviousStep();         break;
    case BP::NEXT_STEP:         goToNextStep();             break;

    // construction bar
    case BP::DEBUG_PEN:               switchMode(Canvas::Mode::PEN);        break;
    case BP::DEBUG_CHANGE_PEN_COLOR:  changePenColor();                     break;
    case BP::DEBUG_LINK:              switchMode(Canvas::Mode::LINK);       break;
    case BP::DEBUG_CHANGE_LINK_COLOR: changeLinkColor();                    break;
    case BP::DEBUG_MOVE:              switchMode(Canvas::Mode::MOVE);       break;
    case BP::DEBUG_ERASER:            switchMode(Canvas::Mode::ERASER);     break;
        
    // control bar
    case BP::DEBUG_SELECT:              switchMode(Canvas::Mode::SELECT);   break;
    case BP::DEBUG_PAN:                 switchMode(Canvas::Mode::PAN);      break;
    case BP::DEBUG_DELETE_SELECTED:     deleteSelected();                   break;
    case BP::DEBUG_DYE_SELECTED:        dyeSelected();                      break;
    case BP::DEBUG_CHANGE_DYE_COLOR:    changeDyeColor();                   break;
    
    // grid settings
    case BP::RESET_CAMERA:  resetCamera();  break;
        
    case BP::NONE: default: break;
    }
}

bool Toolbar::updateKeys(int key){
    // no modifier keys
    switch(key){
    case KEY_P: switchMode(Canvas::Mode::PEN);      return true;
    case KEY_L: switchMode(Canvas::Mode::LINK);     return true;
    case KEY_E: switchMode(Canvas::Mode::ERASER);   return true;
    case KEY_M: switchMode(Canvas::Mode::MOVE);     return true;
    case KEY_S: switchMode(Canvas::Mode::SELECT);   return true;
    case KEY_Q: switchMode(Canvas::Mode::PAN);      return true;
    case KEY_DELETE: deleteSelected();              return true;
    case KEY_R: resetCamera();                      return true;
    default: break;
    }
    return false;
}

bool Toolbar::updateCommandKeys(int key){
    // control
    switch(key){
    case KEY_Z: undo();             return true;
    case KEY_X: deleteSelected();   return true;
    default: break;
    }
    return false;
}

bool Toolbar::updateCommandShiftKeys(int key){
    // control + shift
    switch(key){
    case KEY_Z: redo(); return true;
    default: break;
    }
    return false;
}

void Toolbar::refreshAnchors(){
    float horizontalOffset{(GetScreenWidth() - 1000) / 2.0f};

    mainPanelBounds_ = {horizontalOffset, 0, 1000, 80};

    ioAnchor_ = {horizontalOffset, 0}; 
    gridSettingsAnchor_ = {216 + horizontalOffset, 0};
    algorithmAnchor_ = {480 + horizontalOffset, 0};
    constructionAnchor_ = {0 + horizontalOffset, 40};
    controlAnchor_ = {400 + horizontalOffset, 40};

    hideButtonAnchor_ = {
        algorithmAnchor_.x + 480, 
        algorithmAnchor_.y + 48, 
        24, 
        24
    };

    if(GetScreenWidth() >= 1080){
        hideButtonAnchor_.x = GetScreenWidth() - 32;
        hideButtonAnchor_.y = algorithmAnchor_.y + 8;
    }
}