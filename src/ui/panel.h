#pragma once

#include <vector>
#include <raylib.h>

namespace UI{

class Panel{
public:
    enum class ID{
        TOOLBAR,
        SAVE,
        LOAD,
        CONFIRM_REPLACE,
        SETTINGS
    };

public:
    Panel() = default;
    virtual ~Panel() = default;

    virtual void draw() = 0;
    virtual void update(){} // I think update() isn't needed for most panels

    // only handle inputs from the top panel
    // return true to request poping
    virtual bool handleInput();

protected:
    bool isMousePressedInsidePanel(int button) const;

protected:
    virtual void handleButtonPress() = 0;

    void handleKeyboardShortcuts();
    virtual bool updateKeys(int key){ return false;}
    virtual bool updateCommandKeys(int key){ return false;}
    virtual bool updateCommandShiftKeys(int key){ return false;}

private:
    bool isMouseInsidePanel() const;

protected:
    ID id_;

    Rectangle mainPanelBounds_;
    std::vector<Rectangle> subPanelBounds_;
};

} // namespace UI