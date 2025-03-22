#pragma once

#include <filesystem>

class Canvas;
struct Color;

class GUI{
public:
    enum class Panel{
        NONE,

        SAVE_TO,
        LOAD_FROM,

        RESET_CONFIRM,

        COLOR_PANEL_PEN,
        COLOR_PANEL_LINK,
        COLOR_PANEL_DYE
    };

public:
    GUI();
    ~GUI() = default;

    void draw();
    void update();

    bool isShowingPanel() const{ return panel_ != Panel::NONE;}
    bool isMouseInsidePanel() const;
    void closePanel();

private:
    void drawPanels();
    void switchPanel(Panel panel);

    void drawLoadPanel();
    void drawLoadScrollPanel();
    void updateLoadPanel();
    bool checkLoadFile();
    void checkLoadDirectory();
    void resetLoadDirectory();
    void handleLoadFileButtonPressed();
    void handleLoadFileGuiCancelPressed();
    std::string getFullLoadPath() const{ return (loadDirectory_.path() / loadFilename_).string();}
    void handleLoadFileDirectoryGoBack();

    void drawColorTips(const Color &pen, const Color &link, const Color &dye) const;
    void drawPenColorPanel(Color &color);
    void drawLinkColorPanel(Color &color);
    void drawDyeColorPanel(Color &color);

private:
    void updateKeyboardShortcuts();

    bool updateKeys(int key);
    bool updateCommandKeys(int key);
    bool updateCommandShiftKeys(int key);

private:
    void updateFile();
    void updateView();
    void updateSettings();
    void updateConstruction();
    void updateControls();

private:
    void takeScreenshot();

private:
    Panel panel_;

    // load panel
    std::filesystem::directory_entry loadDirectory_;
    std::string loadFilename_;
};