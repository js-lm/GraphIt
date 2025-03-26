#include "toolbar.h"
#include "system/application.h"
#include "actions_center/actions_center.h"
#include "graph/graph.h"

#include <iomanip>
#include <ctime>
#include <string>
#include <sstream>
#include <raylib.h>
#include <iostream>

using namespace UI;

void Toolbar::switchMode(Canvas::Mode mode){
    // Application::instance().canvas().switchMode(mode);
    // ui.currentSelectedTool = static_cast<int>(mode);
    Application::instance().canvas().switchMode(mode);
    Application::setValue<Setting, int>(Setting::TOOLBAR_CURRENT_SELECTED_TOOL, static_cast<int>(mode));
}

void Toolbar::createNewFile(){
    // TODO: add confirm prompt
    Application::instance().graph().clearGraph();
}

void Toolbar::loadSavedGraph(){
    Application::instance().ui().openPanel(Panel::ID::LOAD);
}

void Toolbar::saveCurrentGraph(){

}

void Toolbar::undo(){ Application::instance().actionCenter().undo();}
void Toolbar::redo(){ Application::instance().actionCenter().redo();}

void Toolbar::takeScreenshot(){
    time_t rawTime{std::time(nullptr)};
    auto timeInfo{*std::localtime(&rawTime)};
    
    std::stringstream fileName;
    fileName << "GraphIt! " 
             << std::put_time(&timeInfo, "%Y-%m-%d %H-%M-%S") 
             << ".png";
    
    TakeScreenshot(fileName.str().c_str());
}

void Toolbar::openSettingMenu(){
    Application::instance().ui().openPanel(Panel::ID::SETTINGS);
}

void Toolbar::deleteSelected(){ 
    bool isSelectingVertex{Application::getValue<Setting, bool>(Setting::TOOLBAR_IS_SELECTING_VERTEX)};
    bool isSelectingEdge{Application::getValue<Setting, bool>(Setting::TOOLBAR_IS_SELECTING_EDGE)};

    if(isSelectingVertex && isSelectingEdge){
        Application::instance().canvas().doBulkDelete();
    }else if(isSelectingVertex){
        Application::instance().canvas().doBulkDeleteVertices();
    }else if(isSelectingEdge){
        Application::instance().canvas().doBulkDeleteEdges();
    }
}

void Toolbar::dyeSelected(){ 
    bool isSelectingVertex{Application::getValue<Setting, bool>(Setting::TOOLBAR_IS_SELECTING_VERTEX)};
    bool isSelectingEdge{Application::getValue<Setting, bool>(Setting::TOOLBAR_IS_SELECTING_EDGE)};

    if(isSelectingVertex && isSelectingEdge){
        Application::instance().canvas().doDye();
    }else if(isSelectingVertex){
        Application::instance().canvas().doDyeVertex();
    }else if(isSelectingEdge){
        Application::instance().canvas().doDyeEdge();
    }
}

void Toolbar::resetCamera(){
    Application::instance().canvas().resetCamera();
}

void Toolbar::changePenColor(){

}

void Toolbar::changeLinkColor(){

}

void Toolbar::changeDyeColor(){

}


void Toolbar::enterAlgorithmMode(){

}

void Toolbar::exitAlgorithmMode(){

}

void Toolbar::goToPreviousStep(){

}

void Toolbar::goToNextStep(){

}
