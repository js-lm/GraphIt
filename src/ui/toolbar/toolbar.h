#pragma once

#include "../panel.h"
#include "canvas/canvas.h"

namespace UI{

class Toolbar : public Panel{
public:
    Toolbar(){
        id_ = ID::TOOLBAR;
        mainPanelBounds_ = {0, 0, 1000, 80};
    };
    ~Toolbar() = default;

    void draw() override;

private:
    void handleButtonPress() override;

    bool updateKeys(int key) override;
    bool updateCommandKeys(int key) override;
    bool updateCommandShiftKeys(int key) override;

private: // utilities
    void switchMode(Canvas::Mode mode);

    void createNewFile();
    void loadSavedGraph();
    void saveCurrentGraph();
    void undo();
    void redo();
    void takeScreenshot();
    
    void openSettingMenu();

    void deleteSelected();
    void dyeSelected();

    void resetCamera();

private:
    void drawIoBar();
    void drawGridSettingsBar();
    void drawAlgorithmBar();
    void drawHideButton();

    void drawConstructionBar();
    void drawControlsBar();

private:
    Vector2 ioAnchor_{0, 0};
    Vector2 gridSettingsAnchor_{216, 0};
    Vector2 algorithmAnchor_{480, 0};
    Vector2 constructionAnchor_{0, 40};
    Vector2 controlAnchor_{400, 40};

private:
    // setting
    bool openSettingMenuPressed_{false};
    bool isHidingUi{false};

    // toolbar - io
    bool newfilePressed_{false};
    bool loadfilePressed_{false};
    bool savefilePressed_{false};
    bool undoPressed_{false};
    bool redoPressed_{false};
    bool screenshotPressed_{false};

    // toolbar - algorithm
    bool runAlgorithmPressed_{false};
    bool exitAlgorithmPressed_{false};
    bool goToPreviousStepPressed_{false};
    bool goToNextStepPressed_{false};
    bool algorithmDropdownBoxEditMode_{false};

    // toolbar - construction
    bool DEBUG_penPressed_{false};
    bool DEBUG_changePenColorPressed_{false};
    bool DEBUG_linkPressed_{false};
    bool DEBUG_changeLinkColorPressed_{false};
    bool DEBUG_movePressed_{false};
    bool DEBUG_eraserPressed_{false};

    // toolbar - controls
    bool DEBUG_selectPressed_{false};
    bool DEBUG_panPressed_{false};
    bool DEBUG_deleteSelectedPressed_{false};
    bool DEBUG_dyeSelectedPressed_{false};
    bool DEBUG_changeDyeColorPressed_{false};
};

} // namespace UI