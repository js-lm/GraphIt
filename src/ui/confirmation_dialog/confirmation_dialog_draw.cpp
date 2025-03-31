#include "confirmation_dialog.hpp"

#include <raylib.h>
#include <raygui.h>

using namespace UI;

void ConfirmationDialog::drawPanel(){
    if(GuiWindowBox(mainPanelBounds_, title_.c_str())) cancel();

    auto originalTextAlignment{GuiGetStyle(LABEL,  TEXT_ALIGNMENT)};
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    GuiLabel({windowAnchor_.x, windowAnchor_.y + 32, textSize_.x + 16, 24}, message_.c_str());
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, originalTextAlignment);
    
    cancelPressed_ = GuiButton({windowAnchor_.x + buttonGap_, windowAnchor_.y + 64, 120, 24}, cancel_.c_str()); 
    proceedPressed_ = GuiButton({windowAnchor_.x + 2 * buttonGap_ + 120, windowAnchor_.y + 64, 120, 24}, proceed_.c_str()); 
}