#pragma once

class Canvas;
struct Color;

class GUI{
public:
    enum class ColorPanelMode{
        NONE,
        PEN,
        LINK,
        DYE
    };

public:
    GUI();
    ~GUI() = default;

    void draw();
    void update();

    bool isShowingPanel() const{ return colorPanelMode_ != ColorPanelMode::NONE;};
    bool isMouseInsidePanel() const;
    void closePanel(){ colorPanelMode_ = ColorPanelMode::NONE;};

private:
    void drawColorPanel();
    void switchColorPanel(ColorPanelMode mode);

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
    ColorPanelMode colorPanelMode_;
};