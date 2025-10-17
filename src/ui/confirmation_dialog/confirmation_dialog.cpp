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
    Vector2 dpiScale{GetWindowScaleDPI()};
    float physicalScreenWidth{static_cast<float>(GetRenderWidth())};
    float physicalScreenHeight{static_cast<float>(GetRenderHeight())};
    
    mainPanelBounds_ = {
        (physicalScreenWidth - (textSize_.x + 8) * dpiScale.x) / (2.0f * dpiScale.x),  
        (physicalScreenHeight - 104 * dpiScale.y) / (2.0f * dpiScale.y),
        textSize_.x + 16,
        96
    };

    windowAnchor_ = {
        mainPanelBounds_.x, 
        mainPanelBounds_.y
    };
}