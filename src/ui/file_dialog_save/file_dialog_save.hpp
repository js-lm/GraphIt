#pragma once

#include "../panel.hpp"

#include <filesystem>

namespace fs = std::filesystem;

namespace UI{

    class FileDialogSave : public Panel{
    public:
        FileDialogSave(){
            id_ = ID::SAVE;
            shouldBlueBackground_ = true;
            refreshAnchors();
            resetPath();
            initSaveFilename();
        };
        ~FileDialogSave() = default;

        void draw() override;

    private:
        void handleButtonPress() override;

        void refreshAnchors() override;
        
    private: // utilities
        void save();

        void resetPath();
        bool tryOpenPath(const std::string &path);
        void synName();
        
        void initSaveFilename();
        void setFilename(const std::string &newName = "");

    private: // draw
        void drawPanel();

    private: // anchors
        Vector2 windowAnchor_;

    private:
        bool directoryTextBoxEditMode_{false};
        char saveDirectoryText_[256]{0};

        bool filenameTextBoxEditMode_{false};
        char filenameTextText_[128]{0};

        bool clearFilenamePressed;
        bool saveButtonPressed;

    private:
        fs::path saveTo_;
    };

} // namespace UI