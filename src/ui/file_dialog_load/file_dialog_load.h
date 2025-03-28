#pragma once

#include "../panel.h"

#include <filesystem>
#include <vector>
#include <optional>

namespace fs = std::filesystem;

struct Rectangle;
struct Vector2;

namespace UI{

class FileDialogLoad : public Panel{
public:
    enum class ButtonPressed{
        NONE,
        BACK,
        SELECT,
        CANCEL
    };

public:
    FileDialogLoad(){
        id_ = ID::LOAD;
        shouldBlueBackground_ = true;
        refreshAnchors();
        resetPath();
    };
    ~FileDialogLoad() = default;

    void draw() override;
    void update() override;

    void refreshAnchors() override;

private:
    void pressButton(ButtonPressed button){ pressedButton_ = button;}
    const ButtonPressed buttonPressed() const{ return pressedButton_;}

    void handleButtonPress() override;

private: // utilities
    void goBack();
    void cancel();
    void loadSelectedFile();
    
    void synNames();
    void fetchUserInput();
    void openSelectedFile();

    void updateFileList(); 
    void resetPath();
    void trySetPath(const std::string &path);
    void trySetFile(const std::string &path);

private: // draw
    void drawPanel();
    void drawScrollPanel();

private: // anchors
    Vector2 windowAnchor_;

private:
    ButtonPressed pressedButton_{ButtonPressed::NONE};

    bool pathEditMode_{false};
    char pathText_[256]{0};
    
    bool filenameEditMode_{false};
    char fileNameText_[128]{0};

    bool fileTypeEditMode_{false};
    int fileTypeIndex_{0};

    int listViewScrollIndex_{0};
    std::optional<int> itemIndex_;

private:
    fs::path currentPath_;
    fs::path selectedFile_;

    std::vector<fs::path> filesList_;
    std::string listViewString_;

    std::string fileExtensionFilter_{".grt"};

private:
    std::optional<float> doubleLeftClickCD_;
};

} // namespace UI