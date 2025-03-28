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
        SETTINGS,
        COLOR,
    };

public:
    Panel() = default;
    virtual ~Panel() = default;

    virtual void draw() = 0;
    virtual void update(){}

    // only handle inputs from the top panel
    virtual void handleInput();

    const Rectangle getMainPanelBounds() const{ return mainPanelBounds_;}

    const bool shouldClosePanel() const{ return shouldClosePanel_;}
    const bool shouldBlurBackground() const{ return shouldBlueBackground_;}

    virtual void refreshAnchors() = 0;

public:
    bool isMousePressedInsidePanel(int button) const;
    bool isMouseInsidePanel() const;

protected:
    virtual void handleButtonPress() = 0;

    void handleKeyboardShortcuts();
    virtual bool updateKeys(int key);
    virtual bool updateCommandKeys(int key){ return false;}
    virtual bool updateCommandShiftKeys(int key){ return false;}

protected:
    ID id_;

    bool shouldClosePanel_{false};
    bool shouldBlueBackground_{false};

    Rectangle mainPanelBounds_;
};

} // namespace UI