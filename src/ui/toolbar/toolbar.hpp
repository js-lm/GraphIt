#pragma once

#include "../panel.hpp"
#include "canvas/canvas.hpp"

namespace UI{

    class Toolbar : public Panel{
    public:
        enum class ButtonPressed{
            NONE,

            // setting
            OPEN_SETTING_MENU,

            // toolbar - io
            NEW_FILE,
            LOAD_FILE,
            SAVE_FILE,
            UNDO,
            REDO,
            SCREENSHOT,

            // toolbar - algorithm
            RUN_ALGORITHM,
            EXIT_ALGORITHM,
            PREVIOUS_STEP,
            NEXT_STEP,

            // toolbar - construction
            DEBUG_PEN,
            DEBUG_CHANGE_PEN_COLOR,
            DEBUG_LINK,
            DEBUG_CHANGE_LINK_COLOR,
            DEBUG_MOVE,
            DEBUG_ERASER,

            // toolbar - controls
            DEBUG_SELECT,
            DEBUG_PAN,
            DEBUG_DELETE_SELECTED,
            DEBUG_DYE_SELECTED,
            DEBUG_CHANGE_DYE_COLOR,
            
            // grid settings
            RESET_CAMERA
        };

    public:
        Toolbar(){ id_ = ID::TOOLBAR;};
        ~Toolbar() = default;

        void draw() override;

        void refreshAnchors() override;

    private:
        void pressButton(ButtonPressed button){ pressedButton_ = button;}
        const ButtonPressed buttonPressed() const{ return pressedButton_;}

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

        void changePenColor();
        void changeLinkColor();
        void changeDyeColor();

        void enterAlgorithmMode();
        void exitAlgorithmMode();
        void goToPreviousStep();
        void goToNextStep();

    private: // draw
        void drawIoBar();
        void drawGridSettingsBar();
        void drawAlgorithmBar();
        void drawHideButton();

        void drawConstructionBar();
        void drawControlsBar();

    private: // anchors
        Vector2 ioAnchor_;
        Vector2 gridSettingsAnchor_;
        Vector2 algorithmAnchor_;
        Vector2 constructionAnchor_;
        Vector2 controlAnchor_;
        Rectangle hideButtonAnchor_;

    private:
        ButtonPressed pressedButton_{ButtonPressed::NONE};

        // setting
        bool isHidingUi_{false};

        // toolbar - algorithm
        bool algorithmDropdownBoxEditMode_{false};
    };

} // namespace UI