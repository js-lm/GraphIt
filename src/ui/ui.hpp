#pragma once

#include "panel.hpp"

#include <vector>
#include <memory>
#include <functional>
#include <string>

namespace UI{

    class Center{
    public:
        Center();
        ~Center() = default;

        void draw();
        void update();

    public:
        bool isMouseOnCanvas() const;
        
        void updatePanelAnchors();

    public:
        void openPanel(Panel::ID id);
        void closeTopPanel();

        void askForConfirmation(
            const std::string &title, 
            const std::string &message, 
            const std::string &proceed,
            const std::string &cancel,
            std::function<void()> proceedAction,
            std::function<void()> cancelAction = [](){}
        );

    private:
        std::vector<std::unique_ptr<Panel>> openedPanels_;
    };  

} // namespace UI