#pragma once

class Canvas;

class GUI{
public:
    GUI() = default;
    ~GUI() = default;

    void draw();
    void update();

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
};