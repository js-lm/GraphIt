#include "toolbar.hpp"
#include "system/application.hpp"
#include "actions_center/actions_center.hpp"
#include "graph/graph.hpp"
#include "io/io.hpp"
#include "system/settings.hpp"

#include <iomanip>
#include <ctime>
#include <string>
#include <sstream>
#include <raylib.h>
#include <iostream>

using namespace UI;

void Toolbar::switchMode(Canvas::Mode mode){
    Application::instance().canvas().switchMode(mode);
    appSettings.toolbarCurrentSelectedTool = static_cast<int>(mode);
}

void Toolbar::createNewFile(){
    Application::instance().ui().askForConfirmation(
        "#193#Are you sure you want to proceed?",
        "This will close the current graph and all the unsaved data will be lost. Proceed?",
        "#147#Proceed",
        "#159#Back",
        [](){
            Application::instance().graph().clearGraph();
        }
    );
}

void Toolbar::loadSavedGraph(){
    Application::instance().ui().openPanel(Panel::ID::LOAD);
}

void Toolbar::saveCurrentGraph(){
    Application::instance().ui().openPanel(Panel::ID::SAVE);
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
    bool isSelectingVertex{appSettings.toolbarIsSelectingVertex};
    bool isSelectingEdge{appSettings.toolbarIsSelectingEdge};

    if(isSelectingVertex && isSelectingEdge){
        Application::instance().canvas().doBulkDelete();
    }else if(isSelectingVertex){
        Application::instance().canvas().doBulkDeleteVertices();
    }else if(isSelectingEdge){
        Application::instance().canvas().doBulkDeleteEdges();
    }
}

void Toolbar::dyeSelected(){ 
    bool isSelectingVertex{appSettings.toolbarIsSelectingVertex};
    bool isSelectingEdge{appSettings.toolbarIsSelectingEdge};

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
    Application::instance().canvas().setZoomBarTimer();
}

void Toolbar::changePenColor(){
    appSettings.colorPanelCallFrom = 1;
    Application::instance().ui().openPanel(Panel::ID::COLOR);
    switchMode(Canvas::Mode::PEN);
}

void Toolbar::changeLinkColor(){
    appSettings.colorPanelCallFrom = 2;
    Application::instance().ui().openPanel(Panel::ID::COLOR);
    switchMode(Canvas::Mode::LINK);
}

void Toolbar::changeDyeColor(){
    appSettings.colorPanelCallFrom = 3;
    Application::instance().ui().openPanel(Panel::ID::COLOR);
}


void Toolbar::enterAlgorithmMode(){

}

void Toolbar::exitAlgorithmMode(){

}

void Toolbar::goToPreviousStep(){

}

void Toolbar::goToNextStep(){

}
