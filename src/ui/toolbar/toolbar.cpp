#include "toolbar.h"
#include "system/application.h"

using namespace UI;

void Toolbar::draw(){
    if(!isHidingUi) DrawRectangleRec(mainPanelBounds_, Fade(WHITE, .5f));

    drawHideButton();
    if(isHidingUi) return;

    drawConstructionBar();
    drawControlsBar();
    drawIoBar();
    drawGridSettingsBar();
    drawAlgorithmBar();
}

void Toolbar::handleButtonPress(){
    if(openSettingMenuPressed_) openSettingMenu();

    if(newfilePressed_) createNewFile();
    if(loadfilePressed_) loadSavedGraph();
    if(savefilePressed_) saveCurrentGraph();
    if(undoPressed_) undo();
    if(redoPressed_) redo();
    if(screenshotPressed_) takeScreenshot();

    if(runAlgorithmPressed_){

    }

    if(exitAlgorithmPressed_){

    }

    if(goToPreviousStepPressed_){

    }

    if(goToNextStepPressed_){

    }

    if(DEBUG_penPressed_) switchMode(Canvas::Mode::PEN);

    if(DEBUG_changePenColorPressed_){

    }

    if(DEBUG_linkPressed_) switchMode(Canvas::Mode::LINK);

    if(DEBUG_changeLinkColorPressed_){

    }

    if(DEBUG_movePressed_) switchMode(Canvas::Mode::MOVE);
    if(DEBUG_eraserPressed_) switchMode(Canvas::Mode::ERASER);
    if(DEBUG_selectPressed_) switchMode(Canvas::Mode::SELECT);
    if(DEBUG_panPressed_) switchMode(Canvas::Mode::PAN);
    if(DEBUG_deleteSelectedPressed_) deleteSelected();
    if(DEBUG_dyeSelectedPressed_) dyeSelected();

    if(DEBUG_changeDyeColorPressed_){

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