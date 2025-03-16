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
    bool updateControlKeys(int key);
    bool updateControlShiftKeys(int key);

private:
    void updateFile();
    void updateView();
    void updateSettings();
    void updateConstruction();
    void updateControls();
};