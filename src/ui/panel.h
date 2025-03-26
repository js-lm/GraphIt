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
    virtual void update(){}

    // only handle inputs from the top panel
    // return true to request poping
    virtual bool handleInput();

    const Rectangle getMainPanelBounds() const{ return mainPanelBounds_;}

    const bool shouldExit() const{ return shouldExit_;}
    const bool shouldBlurBackground() const{ return shouldBlueBackground_;}

public:
    bool isMousePressedInsidePanel(int button) const;
    bool isMouseInsidePanel() const;

protected:
    virtual void handleButtonPress() = 0;

    void handleKeyboardShortcuts();
    virtual bool updateKeys(int key){ return false;}
    virtual bool updateCommandKeys(int key){ return false;}
    virtual bool updateCommandShiftKeys(int key){ return false;}

protected:
    ID id_;

    bool shouldExit_{false};
    bool shouldBlueBackground_{false};

    Rectangle mainPanelBounds_;
    // std::vector<Rectangle> subPanelBounds_;
};

} // namespace UI