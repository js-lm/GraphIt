#include "settings_menu.h"
#include "system/application.h"

#include <raylib.h>
#include <raygui.h>
#include <raymath.h>
#include <cstring>

using namespace UI;

using BP = SettingsMenu::ButtonPressed;

void SettingsMenu::drawMainWindow(){
    const Vector2 &anchor{settingWindowAnchor_};

    shouldExit_ = GuiWindowBox({anchor.x + 0, anchor.y + 0, 688, 480}, "#142#Settings");

    GuiListView({anchor.x + 0, anchor.y + 24, 96, 456}, "General;Graphics;DEBUG", &settingListViewScrollIndex_, &settingListViewActive_);

    if(GuiButton({anchor.x + 480, anchor.y + 424, 192, 32}, "#152#Reset All Settings To Default")){
        pressButton(BP::RESET_ALL);
    }

    if(GuiButton({anchor.x + 480, anchor.y + 376, 192, 32}, "#029#Reset Colors")){
        pressButton(BP::COLOR_RESET);
    }

    if(GuiButton({anchor.x + 480, anchor.y + 344, 192, 32}, "#096#Reset Grid")){
        pressButton(BP::GRID_RESET);
    }

    if(GuiButton({anchor.x + 480, anchor.y + 312, 192, 32}, "#032#Reset Vertex/Edge")){
        pressButton(BP::VISUAL_RESET_SIZE);
    }
}

void SettingsMenu::drawGeneralGraphSettings(){
    const Vector2 &anchor{graphSettingAnchor_};

    bool isDirected{Application::getValue<Setting, bool>(Setting::GRAPH_IS_DIRECTED)};
    bool isWeighted{Application::getValue<Setting, bool>(Setting::GRAPH_IS_WEIGHTED)};
    int precision{Application::getValue<Setting, int>(Setting::GRAPH_WEIGHT_PRECISION)};
    bool isLabeled{Application::getValue<Setting, bool>(Setting::GRAPH_IS_LABELED)};

    GuiGroupBox({anchor.x + 0, anchor.y + 0, 392, 128}, "Graph Settings");

    GuiToggle({anchor.x + 16, anchor.y + 16, 104, 24}, "#210#Is Directed", &isDirected);

    GuiToggle({anchor.x + 16, anchor.y + 48, 104, 24}, "#178#Is Weighted", &isWeighted);
    GuiLabel({anchor.x + 24, anchor.y + 96, 96, 24}, "Weight Precision");
    if(GuiSpinner({anchor.x + 16, anchor.y + 72, 104, 24}, NULL, &precision, 0, 100, weightPrecisionEditMode_)){
        weightPrecisionEditMode_ = !weightPrecisionEditMode_;
    }

    GuiToggle({anchor.x + 144, anchor.y + 16, 104, 24}, "#031#Is Labeled", &isLabeled);
    
    GuiLabel({anchor.x + 144, anchor.y + 64, 72, 24}, "#191#Warning!");
    GuiLabel({anchor.x + 144, anchor.y + 80, 248, 24}, "This will reset the undo/redo history as well!");
    if(GuiButton({anchor.x + 144, anchor.y + 40, 104, 24}, "#211#Reset Labels")){
        pressButton(BP::GRAPH_RESET_LABEL);
    }

    Application::setValue<Setting, bool>(Setting::GRAPH_IS_DIRECTED, isDirected);
    Application::setValue<Setting, bool>(Setting::GRAPH_IS_WEIGHTED, isWeighted);
    Application::setValue<Setting, int>(Setting::GRAPH_WEIGHT_PRECISION, Clamp(precision, 0, 5));
    Application::setValue<Setting, bool>(Setting::GRAPH_IS_LABELED, isLabeled);
}

void SettingsMenu::drawGeneralColorPreferences(){
    const Vector2 &anchor{colorPreferencesAnchor_};

    Color colorPickerValue{Application::getValue<Setting, Color>(Setting::COLOR_PREFERENCE_PICKER)};

    GuiGroupBox({anchor.x + 0, anchor.y + 0, 352, 272}, "Color Preferences");
    GuiColorPicker({anchor.x + 216, anchor.y + 32, 96, 96}, NULL, &colorPickerValue);
    GuiLine({anchor.x + 8, anchor.y + 120, 176, 32}, NULL);

    GuiGroupBox({anchor.x + 16, anchor.y + 16, 160, 104}, "Vertex");
    GuiLabel({anchor.x + 24, anchor.y + 24, 64, 24}, "Start/End:");
    if(GuiButton({anchor.x + 88, anchor.y + 24, 24, 24}, NULL)){
        pressButton(BP::COLOR_VERTEX_START);
    }
    if(GuiButton({anchor.x + 112, anchor.y + 24, 24, 24}, NULL)){
        pressButton(BP::COLOR_VERTEX_END);
    }

    GuiLabel({anchor.x + 24, anchor.y + 56, 104, 24}, "Visited/Unvisited:");
    if(GuiButton({anchor.x + 120, anchor.y + 56, 24, 24}, NULL)){
        pressButton(BP::COLOR_VERTEX_VISTED);
    }
    if(GuiButton({anchor.x + 144, anchor.y + 56, 24, 24}, NULL)){
        pressButton(BP::COLOR_VERTEX_UNVISITED);
    }

    GuiLabel({anchor.x + 24, anchor.y + 88, 96, 24}, "Current/Special:");
    if(GuiButton({anchor.x + 120, anchor.y + 88, 24, 24}, NULL)){
        pressButton(BP::COLOR_VERTEX_CURRENT);
    }
    if(GuiButton({anchor.x + 144, anchor.y + 88, 24, 24}, NULL)){
        pressButton(BP::COLOR_VERTEX_SPECIAL);
    }

    GuiGroupBox({anchor.x + 16, anchor.y + 152, 160, 104}, "Edge");
    GuiLabel({anchor.x + 24, anchor.y + 160, 104, 24}, "Visited/Unvisited:");
    if(GuiButton({anchor.x + 120, anchor.y + 160, 24, 24}, NULL)){
        pressButton(BP::COLOR_EDGE_VISTED);
    }
    if(GuiButton({anchor.x + 144, anchor.y + 160, 24, 24}, NULL)){
        pressButton(BP::COLOR_EDGE_UNVISITED);
    }

    GuiLabel({anchor.x + 24, anchor.y + 192, 96, 24}, "Current/Path:");
    if(GuiButton({anchor.x + 120, anchor.y + 192, 24, 24}, NULL)){
        pressButton(BP::COLOR_EDGE_CURRENT);
    }
    if(GuiButton({anchor.x + 144, anchor.y + 192, 24, 24}, NULL)){
        pressButton(BP::COLOR_EDGE_PATH);
    }

    GuiLabel({anchor.x + 24, anchor.y + 224, 88, 24}, "Block/Special:");
    if(GuiButton({anchor.x + 104, anchor.y + 224, 24, 24}, NULL)){
        pressButton(BP::COLOR_EDGE_BLOCK);
    }
    if(GuiButton({anchor.x + 128, anchor.y + 224, 24, 24}, NULL)){
        pressButton(BP::COLOR_EDGE_SPECIAL);
    }

    GuiLabel({anchor.x + 216, anchor.y + 152, 120, 24}, "Presets:");
    if(GuiButton({anchor.x + 216, anchor.y + 168, 24, 24}, NULL)){
        pressButton(BP::COLOR_PRESET_1);
    }
    if(GuiButton({anchor.x + 240, anchor.y + 168, 24, 24}, NULL)){
        pressButton(BP::COLOR_PRESET_2);
    }
    if(GuiButton({anchor.x + 264, anchor.y + 168, 24, 24}, NULL)){
        pressButton(BP::COLOR_PRESET_3);
    }
    if(GuiButton({anchor.x + 288, anchor.y + 168, 24, 24}, NULL)){
        pressButton(BP::COLOR_PRESET_4);
    }
    if(GuiButton({anchor.x + 216, anchor.y + 192, 24, 24}, NULL)){
        pressButton(BP::COLOR_PRESET_5);
    }
    if(GuiButton({anchor.x + 240, anchor.y + 192, 24, 24}, NULL)){
        pressButton(BP::COLOR_PRESET_6);
    }
    if(GuiButton({anchor.x + 264, anchor.y + 192, 24, 24}, NULL)){
        pressButton(BP::COLOR_PRESET_7);
    }
    if(GuiButton({anchor.x + 288, anchor.y + 192, 24, 24}, NULL)){
        pressButton(BP::COLOR_PRESET_8);
    }
    if(GuiButton({anchor.x + 216, anchor.y + 216, 24, 24}, NULL)){
        pressButton(BP::COLOR_PRESET_9);
    }
    if(GuiButton({anchor.x + 240, anchor.y + 216, 24, 24}, NULL)){
        pressButton(BP::COLOR_PRESET_10);
    }
    if(GuiButton({anchor.x + 264, anchor.y + 216, 24, 24}, NULL)){
        pressButton(BP::COLOR_PRESET_11);
    }
    if(GuiButton({anchor.x + 288, anchor.y + 216, 24, 24}, NULL)){
        pressButton(BP::COLOR_PRESET_12);
    }

    Application::setValue<Setting, Color>(Setting::COLOR_PREFERENCE_PICKER, colorPickerValue);
}

void SettingsMenu::drawGeneralVertexEdgeSettings(){
    const Vector2 &anchor{vertexEdgeSettingsAnchor_};

    float radius{Application::getValue<Setting, float>(Setting::GRAPH_VERTEX_RADIUS)};
    float thickness{Application::getValue<Setting, float>(Setting::GRAPH_EDGE_THICKNESS)};
    // TODO: should I move these outside?
    const float minRadius{5.0f};
    const float maxRadius{50.0f};
    const float minThickness{2.5f};

    GuiGroupBox({anchor.x + 0, anchor.y + 0, 152, 128}, "Vertex/Edge Settings");

    GuiLabel({anchor.x + 16, anchor.y + 8, 120, 24}, "Vertex Radius:");
    GuiSlider({anchor.x + 16, anchor.y + 24, 120, 16}, NULL, NULL, &radius, minRadius, maxRadius);
    if(GuiValueBox({anchor.x + 16, anchor.y + 40, 40, 16}, NULL, &vertexRadiusValueBoxTempValue_, minRadius, maxRadius, vertexRadiusValueBoxEditMode_)){
        vertexRadiusValueBoxEditMode_ = !vertexRadiusValueBoxEditMode_;
        radius = static_cast<float>(vertexRadiusValueBoxTempValue_);
    }else if(!vertexRadiusValueBoxEditMode_){
        vertexRadiusValueBoxTempValue_ = radius;
    }

    GuiLabel({anchor.x + 16, anchor.y + 64, 120, 24}, "Edge Thickness:");
    GuiSlider({anchor.x + 16, anchor.y + 80, 120, 16}, NULL, NULL, &thickness, minThickness, radius);
    if(GuiValueBox({anchor.x + 16, anchor.y + 96, 40, 16}, NULL, &edgeThicknessValueBoxTempValue_, minThickness, radius, edgeThicknessValueBoxEditMode_)){
        edgeThicknessValueBoxEditMode_ = !edgeThicknessValueBoxEditMode_;
        thickness = static_cast<float>(edgeThicknessValueBoxTempValue_);
    }else if(!edgeThicknessValueBoxEditMode_){
        edgeThicknessValueBoxTempValue_ = thickness;
    }

    Application::setValue<Setting, float>(Setting::GRAPH_VERTEX_RADIUS, Clamp(radius, minRadius, maxRadius));
    Application::setValue<Setting, float>(Setting::GRAPH_EDGE_THICKNESS, Clamp(thickness, minThickness, radius));
}

void SettingsMenu::drawGeneralGridSetting(){
    const Vector2 &anchor{gridSettingsAnchor_};

    int cellSize{Application::getValue<Setting, int>(Setting::GRID_CELL_SIZE)};
    int subdivisionSize{Application::getValue<Setting, int>(Setting::GRID_SUBDIVISION_SIZE)};
    float cellSizeFLOAT{static_cast<float>(cellSize)};
    float subdivisionSizeFLOAT{static_cast<float>(subdivisionSize)};
    // TODO: should I move these outside?
    const int minCellSize{8};
    const int maxCellSize{48};
    const int minSubdivisionSize{3};
    const int maxSubdivisionSize{10};

    GuiGroupBox({anchor.x + 0, anchor.y + 0, 192, 96}, "Grid Settings");

    GuiLabel({anchor.x + 16, anchor.y + 8, 120, 24}, "Cell Size:");
    GuiSlider({anchor.x + 16, anchor.y + 24, 120, 16}, NULL, NULL, &cellSizeFLOAT, minCellSize, maxCellSize);
    cellSize = cellSizeFLOAT;
    if(GuiValueBox({anchor.x + 136, anchor.y + 24, 40, 16}, NULL, &cellSizeValueBoxTempValue_, minCellSize, maxCellSize, cellSizeValueBoxEditMode_)){
        cellSizeValueBoxEditMode_ = !cellSizeValueBoxEditMode_;
        cellSize = cellSizeValueBoxTempValue_;
    }else if(!cellSizeValueBoxEditMode_){
        cellSizeValueBoxTempValue_ = cellSize;
    }

    GuiLabel({anchor.x + 16, anchor.y + 48, 120, 24}, "Subdivision Size:");
    GuiSlider({anchor.x + 16, anchor.y + 64, 120, 16}, NULL, NULL, &subdivisionSizeFLOAT, minSubdivisionSize, maxSubdivisionSize);
    subdivisionSize = subdivisionSizeFLOAT;
    if(GuiValueBox({anchor.x + 136, anchor.y + 64, 40, 16}, NULL, &subdivisionSizeValueBoxTempValue_, 0, 100, subdivisionSizeValueBoxEditMode_)){
        subdivisionSizeValueBoxEditMode_ = !subdivisionSizeValueBoxEditMode_;
        subdivisionSize = subdivisionSizeValueBoxTempValue_;
    }else if(!subdivisionSizeValueBoxEditMode_){
        subdivisionSizeValueBoxTempValue_ = subdivisionSize;
    }

    Application::setValue<Setting, int>(Setting::GRID_CELL_SIZE, Clamp(cellSize, minCellSize, maxCellSize));
    Application::setValue<Setting, int>(Setting::GRID_SUBDIVISION_SIZE, Clamp(subdivisionSize, minSubdivisionSize, maxSubdivisionSize));
}