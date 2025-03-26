#include "color_panel.h"
#include "system/application.h"

#include <raylib.h>
#include <raygui.h>
#include <cstring>

using namespace UI;

using BP = ColorPanel::ButtonPressed;

void ColorPanel::drawPanel(){
    const Vector2 &anchor{panelAnchor_};

    if(GuiWindowBox({anchor.x + 0, anchor.y + 0, 336, 208}, "Color Panel")){
        pressButton(BP::CANCEL);
    }

    GuiColorPicker({anchor.x + 16, anchor.y + 40, 96, 96}, NULL, &newColor_);

    GuiLabel({anchor.x + 16, anchor.y + 144, 32, 24}, "RBG: ");
    static char r[16];
    sprintf(r, "%3d", newColor_.r);
    GuiLabel({anchor.x + 56, anchor.y + 144, 24, 24}, r);
    static char g[16];
    sprintf(g, "%3d", newColor_.g);
    GuiLabel({anchor.x + 80, anchor.y + 144, 24, 24}, g);
    static char b[16];
    sprintf(b, "%3d", newColor_.b);
    GuiLabel({anchor.x + 104, anchor.y + 144, 24, 24}, b);

    GuiLabel({anchor.x + 144, anchor.y + 40, 64, 24}, "Old Color:");
    GuiPanel({anchor.x + 200, anchor.y + 40, 24, 24}, NULL);
    DrawRectangle(anchor.x + 200, anchor.y + 40, 24, 24, oldColor_);
    DrawRectangleLinesEx({anchor.x + 200, anchor.y + 40, 24, 24}, 2.0f, ColorBrightness(oldColor_, -.3f));

    GuiLabel({anchor.x + 237, anchor.y + 40, 64, 24}, "New Color:");
    GuiPanel({anchor.x + 296, anchor.y + 40, 24, 24}, NULL);
    DrawRectangle(anchor.x + 296, anchor.y + 40, 24, 24, newColor_);
    DrawRectangleLinesEx({anchor.x + 296, anchor.y + 40, 24, 24}, 2.0f, ColorBrightness(newColor_, -.3f));

    GuiGroupBox({anchor.x + 144, anchor.y + 80, 176, 80}, "Preset Colors");
    if(GuiButton({anchor.x + 160, anchor.y + 96, 24, 24}, NULL)){
        pressButton(BP::COLOR_1);
    }
    if(GuiButton({anchor.x + 184, anchor.y + 96, 24, 24}, NULL)){
        pressButton(BP::COLOR_2);
    }
    if(GuiButton({anchor.x + 208, anchor.y + 96, 24, 24}, NULL)){
        pressButton(BP::COLOR_3);
    }
    if(GuiButton({anchor.x + 232, anchor.y + 96, 24, 24}, NULL)){
        pressButton(BP::COLOR_4);
    }
    if(GuiButton({anchor.x + 256, anchor.y + 96, 24, 24}, NULL)){
        pressButton(BP::COLOR_5);
    }
    if(GuiButton({anchor.x + 280, anchor.y + 96, 24, 24}, NULL)){
        pressButton(BP::COLOR_6);
    }
    if(GuiButton({anchor.x + 160, anchor.y + 120, 24, 24}, NULL)){
        pressButton(BP::COLOR_7);
    }
    if(GuiButton({anchor.x + 184, anchor.y + 120, 24, 24}, NULL)){
        pressButton(BP::COLOR_8);
    }
    if(GuiButton({anchor.x + 208, anchor.y + 120, 24, 24}, NULL)){
        pressButton(BP::COLOR_9);
    }
    if(GuiButton({anchor.x + 232, anchor.y + 120, 24, 24}, NULL)){
        pressButton(BP::COLOR_10);
    }
    if(GuiButton({anchor.x + 256, anchor.y + 120, 24, 24}, NULL)){
        pressButton(BP::COLOR_11);
    }
    if(GuiButton({anchor.x + 280, anchor.y + 120, 24, 24}, NULL)){
        pressButton(BP::COLOR_12);
    }

    GuiLine({anchor.x + 8, anchor.y + 160, 320, 16}, NULL);
    if(GuiButton({anchor.x + 176, anchor.y + 176, 64, 24}, "#113#Back")){
        pressButton(BP::CANCEL);
    }
    if(GuiButton({anchor.x + 96, anchor.y + 176, 64, 24}, "#112#Save")){
        pressButton(BP::SAVE);
    }
}