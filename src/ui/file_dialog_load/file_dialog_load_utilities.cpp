#include "file_dialog_load.hpp"
#include "system/application.hpp"
#include "system/terminal_prefix.hpp"
#include "io/io.hpp"

#include <algorithm>
#include <iostream>
#include <cstring>

using namespace UI;

void FileDialogLoad::goBack(){
    if(currentPath_.has_parent_path()){
        currentPath_ = currentPath_.parent_path();
        synNames();
    }
}

void FileDialogLoad::cancel(){
    shouldClosePanel_ = true;
}

void FileDialogLoad::loadSelectedFile(){
    Application::instance().ui().askForConfirmation(
        "#193#Are you sure you want to proceed?",
        "This will replace the current graph and all the unsaved data will be lost. Proceed?",
        "#147#Proceed",
        "#159#Back",
        [&](){
            printFilesystemPrefix();
            std::cout << "Trying to open " << selectedFile_.string() << std::endl;
            if(Application::instance().serializer().load(selectedFile_.string())){
                cancel();
            }
        }
    );
}

void FileDialogLoad::openSelectedFile(){
    if(itemIndex_
    && itemIndex_.value() >= 0 
    && itemIndex_.value() < static_cast<int>(filesList_.size())
    ){
        auto selectedItem{filesList_[itemIndex_.value()]};

        if(fs::is_directory(selectedItem)){
            currentPath_ = selectedItem;
            selectedFile_ = "";
        }else{
            selectedFile_ = selectedItem;
            loadSelectedFile();
        }

        itemIndex_ = std::nullopt;
        synNames();
    }
}

void FileDialogLoad::synNames(){
    strncpy(pathText_, currentPath_.string().c_str(), sizeof(pathText_) - 1);
    pathText_[sizeof(pathText_) - 1] = '\0';
    strncpy(fileNameText_, selectedFile_.filename().string().c_str(), sizeof(fileNameText_) - 1);
    fileNameText_[sizeof(fileNameText_) - 1] = '\0';
}

void FileDialogLoad::resetPath(){
    if(fs::exists(fs::current_path() / "Saves")
    // || fs::create_directory(fs::current_path() / "Saves")
    ){
        currentPath_ = fs::current_path() / "Saves";
    }else{
        currentPath_ = fs::current_path();
    }

    selectedFile_ = "";

    synNames();

    printFilesystemPrefix();
    std::cout << "Reset path to " << currentPath_.string() << std::endl;
}

void FileDialogLoad::updateFileList(){
    auto oldSize{filesList_.size()};
    filesList_.clear();
    filesList_.reserve(oldSize);
    
    for(const auto &item : fs::directory_iterator(currentPath_)){
        const auto &filename{item.path().filename().string()};

        if(!fs::is_directory(item)
        // TODO: see if there a way to get rid of this
        && (filename.find('#') != std::string::npos 
         || filename.find(';') != std::string::npos
         || !item.path().extension().string().ends_with(fileExtensionFilter_))
        ){
            continue;
        }

        filesList_.push_back(item.path());
    }

    std::sort(
        filesList_.begin(), 
        filesList_.end(), 
        [](const fs::path &directoryA, const fs::path &directoryB){
            bool isADirectory{fs::is_directory(directoryA)};
            bool isBDirectory{fs::is_directory(directoryB)};
            
            if(isADirectory != isBDirectory){
                return isADirectory;
            }
            return directoryA < directoryB;
        }
    );

    if(itemIndex_ >= filesList_.size()) itemIndex_ = std::nullopt;

    listViewString_.clear();

    for(const auto &file : filesList_){
        std::string iconID;
        if(fs::is_directory(file)){
            iconID = "#217#";
        }else if(fs::is_regular_file(file)){
            if(file.extension().string().ends_with(".grt")){
                iconID = "#255#";
            }else{
                iconID = "#218#";
            }
        }

        listViewString_ += iconID + file.filename().string() + ";";
    }
    if(!listViewString_.empty()) listViewString_.pop_back();
}

void FileDialogLoad::trySetPath(const std::string &path){
    if(fs::is_directory(path)){
        currentPath_ = path;
        itemIndex_ = std::nullopt;
    }else{
        resetPath();
    }
}

void FileDialogLoad::trySetFile(const std::string &path){
    for(const auto &item : fs::directory_iterator(currentPath_)){
        if(item.path().filename().string() == path){
            selectedFile_ = item.path();
            itemIndex_ = std::nullopt;
            return;
        }
    }  
}