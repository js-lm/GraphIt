#include "toolbar.h"
#include "system/application.h"

using namespace UI;

void Toolbar::draw(){
    if(!isHidingUi_) DrawRectangleRec(mainPanelBounds_, Fade(WHITE, .5f));

    drawHideButton();
    if(isHidingUi_) return;

    pressButton(ButtonPressed::NONE);

    drawConstructionBar();
    drawControlsBar();
    drawIoBar();
    drawGridSettingsBar();
    drawAlgorithmBar();
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
    // No Modifier Keys
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
    // Control
    switch(key){
    case KEY_Z: undo();             return true;
    case KEY_X: deleteSelected();   return true;
    default: break;
    }
    return false;
}

bool Toolbar::updateCommandShiftKeys(int key){
    // Control + Shift
    switch(key){
    case KEY_Z: redo(); return true;
    default: break;
    }
    return false;
}