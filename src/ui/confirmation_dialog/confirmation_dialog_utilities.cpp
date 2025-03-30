#include "confirmation_dialog.hpp"
#include "system/settings.hpp"

using namespace UI;

void ConfirmationDialog::confirm(){
    proceedAction_();
    shouldClosePanel_ = true;
}

void ConfirmationDialog::cancel(){
    cancelAction_();
    shouldClosePanel_ = true;
}