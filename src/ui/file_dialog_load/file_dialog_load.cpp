#include "file_dialog_load.h"
#include "system/application.h"

using namespace UI;

void FileDialogLoad::draw(){
    pressButton(ButtonPressed::NONE);
    drawPanel();
}

void FileDialogLoad::handleButtonPress(){
    using BP = ButtonPressed;

    switch(buttonPressed()){
        // IO bar
        case BP::BACK:      goBack();       break;

        case BP::SELECT:    selectFile();   break;
        case BP::CANCEL:    cancel();       break;
           
        case BP::NONE: default: break;
    }
}