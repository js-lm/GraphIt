#include "confirmation_dialog.hpp"
#include "system/settings.hpp"

using namespace UI;

#include <raygui.h>

ConfirmationDialog::ConfirmationDialog(
    const std::string &title, 
    const std::string &message, 
    const std::string &proceed,
    const std::string &cancel,
    std::function<void()> proceedAction,
    std::function<void()> cancelAction
)
    : title_(title)
    , message_(message)
    , proceed_(proceed)
    , cancel_(cancel)
    , proceedAction_(std::move(proceedAction))
    , cancelAction_(std::move(cancelAction))
{
    id_ = ID::CONFIRM;
    shouldBlueBackground_ = true;
    textSize_ = MeasureTextEx(GuiGetFont(), message_.c_str(), GuiGetStyle(DEFAULT, TEXT_SIZE), GuiGetStyle(DEFAULT, TEXT_SPACING));
    refreshAnchors();
    buttonGap_ = (mainPanelBounds_.width - 240) / 3.0f;
}

void ConfirmationDialog::handleButtonPress(){
    if(proceedPressed_) confirm();
    if(cancelPressed_) cancel();
}

void ConfirmationDialog::refreshAnchors(){
    mainPanelBounds_ = {
        (GetScreenWidth() - (textSize_.x + 8)) / 2.0f,  
        (GetScreenHeight() - 104) / 2.0f,
        textSize_.x + 16,
        96
    };

    windowAnchor_ = {
        mainPanelBounds_.x, 
        mainPanelBounds_.y
    };
}