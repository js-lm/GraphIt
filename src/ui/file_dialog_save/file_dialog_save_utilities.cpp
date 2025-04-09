#include "file_dialog_save.hpp"
#include "system/application.hpp"
#include "io/io.hpp"
#include "system/terminal_prefix.hpp"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <string>
#include <sstream>

using namespace UI;

void FileDialogSave::save(){
    if(tryOpenPath(saveDirectoryText_)){
        fs::path fullPath{saveTo_ / filenameTextText_};
        fs::path finalPathName{fullPath};
        size_t trial{1};

        while(fs::exists(finalPathName) || fs::exists(finalPathName.string() + ".grt")){
            finalPathName = saveTo_ / fullPath.stem();

            finalPathName += " (" + std::to_string(trial++) + ")" + ".grt";

            printFilesystemPrefix();
            std::cout << "File already exists. Creating a new name... " 
                      << finalPathName << std::endl;
        }

        printFilesystemPrefix();
        std::cout << "Saving " << finalPathName << std::endl;

        try{
            shouldClosePanel_ = Application::instance().serializer().save(finalPathName.string());
        }catch(const std::runtime_error &error){
            Application::instance().ui().askForConfirmation(
                "Warning",
                error.what(),
                "#147#Ok",
                "",
                [](){}
            );

            // TODO: There should also be warning boxes
        }
        
    }else{
        printErrorPrefix();
        std::cerr << "Unable to open " << saveTo_.string() << std::endl;
    }
}

void FileDialogSave::resetPath(){
    saveTo_ = fs::current_path() / "Saves";
    synName();
}

bool FileDialogSave::tryOpenPath(const std::string &path){
    if(fs::is_directory(path)){
        if(saveTo_.string() != path){
            saveTo_ = path;
        }
        return true;
    }

    printErrorPrefix();
    if(fs::create_directory(path)){
        std::cout << "Directory created successfully! "
                  << path << std::endl;
        saveTo_ = path;
        return true;
    }
    
    std::cerr << "Failed to create directory" << std::endl;
    
    return false;
}

void FileDialogSave::synName(){
    strncpy(saveDirectoryText_, saveTo_.string().c_str(), sizeof(saveDirectoryText_) - 1);
    saveDirectoryText_[sizeof(saveDirectoryText_) - 1] = '\0';
}

void FileDialogSave::initSaveFilename(){
    time_t rawTime{std::time(nullptr)};
    auto timeInfo{*std::localtime(&rawTime)};
    
    std::stringstream fileName;
    fileName << "Save - " 
             << std::put_time(&timeInfo, "%Y%m%d%H%M%S")
             << ".grt";

    setFilename(fileName.str());
}

void FileDialogSave::setFilename(const std::string &newName){
    strncpy(filenameTextText_, newName.c_str(), sizeof(filenameTextText_) - 1);
    filenameTextText_[sizeof(filenameTextText_) - 1] = '\0';
}