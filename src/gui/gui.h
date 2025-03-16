#pragma once

class Canvas;

class GUI{
public:
    GUI() = default;
    ~GUI() = default;

    void draw();
    void update();

private:
    void manageCanvasMouseBlocking();
    
private:
    void updateKeyboardShortcuts();

private:
    void updateFile();
    void updateView();
    void updateSettings();
    void updateConstruction();
    void updateControls();
};