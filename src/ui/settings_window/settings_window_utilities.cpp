#include "file_dialog_load.h"
#include "system/application.h"

using namespace UI;

void FileDialogLoad::goBack(){

}

void FileDialogLoad::selectFile(){

}

void FileDialogLoad::cancel(){
    Application::instance().ui().closeTopPanel();
}