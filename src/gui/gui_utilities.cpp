#include "gui.h"

#include <iomanip>
#include <ctime>
#include <string>
#include <sstream>
#include <raylib.h>

void GUI::takeScreenshot(){
    time_t rawTime{std::time(nullptr)};
    auto timeInfo{*std::localtime(&rawTime)};
    
    std::stringstream fileName;
    fileName << "GraphIt! " 
             << std::put_time(&timeInfo, "%Y-%m-%d %H-%M-%S") 
             << ".png";
    
    TakeScreenshot(fileName.str().c_str());
}