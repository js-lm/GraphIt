#pragma once

#include "../panel.hpp"

#include <string>
#include <functional>

namespace UI{

    class ConfirmationDialog : public Panel{
    public:
        ConfirmationDialog() = delete;
        ConfirmationDialog(
            const std::string &title, 
            const std::string &message, 
            const std::string &proceed,
            const std::string &cancel,
            std::function<void()> proceedAction = [](){},
            std::function<void()> cancelAction = [](){}
        );
        ~ConfirmationDialog() = default;

        void draw() override{ drawPanel();}

    private:
        void handleButtonPress() override;

        void refreshAnchors() override;
        
    private: // utilities
        void confirm();
        void cancel();

    private: // draw
        void drawPanel();

    private: // anchors
        Vector2 windowAnchor_;

    private:
        bool proceedPressed_;
        bool cancelPressed_;

    private:
        std::string title_;
        std::string message_; 
        std::string proceed_;
        std::string cancel_;

        std::function<void()> proceedAction_;
        std::function<void()> cancelAction_;

        Vector2 textSize_;
        float buttonGap_;
    };

} // namespace UI