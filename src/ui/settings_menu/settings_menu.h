#pragma once

#include "../panel.h"

#include <string>

struct Rectangle;
struct Vector2;

namespace UI{

class SettingsMenu : public Panel{
public:
    enum class ButtonPressed{
        NONE,

        // graph settings
        GRAPH_RESET_LABEL,

        // vertex/edge settings
        VISUAL_RESET_SIZE,

        // color preferences
        COLOR_RESET,
        COLOR_VERTEX_START,
        COLOR_VERTEX_END,
        COLOR_VERTEX_VISTED,
        COLOR_VERTEX_UNVISITED,
        COLOR_VERTEX_CURRENT,
        COLOR_VERTEX_SPECIAL,
        COLOR_EDGE_VISTED,
        COLOR_EDGE_UNVISITED,
        COLOR_EDGE_CURRENT,
        COLOR_EDGE_PATH,
        COLOR_EDGE_BLOCK,
        COLOR_EDGE_SPECIAL,
        COLOR_PRESET_1,
        COLOR_PRESET_2,
        COLOR_PRESET_3,
        COLOR_PRESET_4,
        COLOR_PRESET_5,
        COLOR_PRESET_6,
        COLOR_PRESET_7,
        COLOR_PRESET_8,
        COLOR_PRESET_9,
        COLOR_PRESET_10,
        COLOR_PRESET_11,
        COLOR_PRESET_12,
        
        // grid setting
        GRID_RESET,

        // reset all
        RESET_ALL,
    };

public:
    SettingsMenu(){
        id_ = ID::SETTINGS;

        shouldBlueBackground_ = true;

        mainPanelBounds_ = {
            settingWindowAnchor_.x, 
            settingWindowAnchor_.y,
            688.0f,
            480.0f
        };
        
    };
    ~SettingsMenu() = default;

    void draw() override;

private:
    void pressButton(ButtonPressed button){ pressedButton_ = button;}
    const ButtonPressed buttonPressed() const{ return pressedButton_;}

    void handleButtonPress() override;

private: // utilities

    // general
    void resetLabel();
    void resetVertexEdgeSize();
    void resetColor();
    void selectButton(ButtonPressed button);
    void changeSelectedButtonColor(ButtonPressed button);
    void resetGrid();
    void resetALL();

private: // draw
    void drawMainWindow();

    // general
    void drawGeneralGraphSettings();
    void drawGeneralColorPreferences();
    void drawGeneralVertexEdgeSettings();
    void drawGeneralGridSetting();

private: // anchors
    Vector2 settingWindowAnchor_{168, 96};

    // general
    Vector2 graphSettingAnchor_{280, 144};
    Vector2 colorPreferencesAnchor_{280, 288};
    Vector2 vertexEdgeSettingsAnchor_{688, 144};
    Vector2 gridSettingsAnchor_{648, 288};

private:
    ButtonPressed pressedButton_{ButtonPressed::NONE};

    bool settingsWindowActive_{false};
    int settingListViewScrollIndex_{0};
    int settingListViewActive_;

private: // general
    // graph settings
    bool weightPrecisionEditMode_{false};

    // vertex/edge settings
    int vertexRadiusValueBoxTempValue_{0};
    bool vertexRadiusValueBoxEditMode_{false};
    int edgeThicknessValueBoxTempValue_{0};
    bool edgeThicknessValueBoxEditMode_{false};

    // grid settings
    int cellSizeValueBoxTempValue_{0};
    bool cellSizeValueBoxEditMode_{false};
    int subdivisionSizeValueBoxTempValue_{0};
    bool subdivisionSizeValueBoxEditMode_{false};
};

} // namespace UI