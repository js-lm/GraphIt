#pragma once

#include "panel.h"

#include <vector>
#include <memory>

namespace UI{

class Center{
public:
    Center();
    ~Center() = default;

    void draw();
    void update();

private:
    void openPanel(Panel::ID id);
    void closeTopPanel();

private:
    std::vector<std::unique_ptr<Panel>> openedPanels_;
};

} // namespace UI