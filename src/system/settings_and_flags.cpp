#include "system/application.h"

#include <raylib.h>
#include <stdexcept>

std::unordered_map<Setting, Application::SettingValue> Application::settings_;
std::unordered_map<Flag, Application::SettingValue> Application::flags_;

void Application::initSettings(){
    // grid settings
    settings_[Setting::GRID_SHOW] = false;
    settings_[Setting::GRID_IS_SNAP_TO_GRID] = false;
    settings_[Setting::GRID_CELL_SIZE] = 24;
    settings_[Setting::GRID_SUBDIVISION_SIZE] = 3;

    // graph settings
    settings_[Setting::GRAPH_IS_DIRECTED] = true;
    settings_[Setting::GRAPH_IS_WEIGHTED] = true;
    settings_[Setting::GRAPH_WEIGHT_PRECISION] = 0;
    settings_[Setting::GRAPH_IS_LABELED] = true;

    // graph visual
    settings_[Setting::GRAPH_VERTEX_RADIUS] = 20.0f;
    settings_[Setting::GRAPH_EDGE_THICKNESS] = 7.0f;

    // vertex colors
    settings_[Setting::COLOR_VERTEX_START] = GREEN;
    settings_[Setting::COLOR_VERTEX_END] = RED;
    settings_[Setting::COLOR_VERTEX_VISITED] = BLUE;
    settings_[Setting::COLOR_VERTEX_UNVISITED] = LIGHTGRAY;
    settings_[Setting::COLOR_VERTEX_CURRENT] = YELLOW;
    settings_[Setting::COLOR_VERTEX_SPECIAL] = PURPLE;

    // edge colors
    settings_[Setting::COLOR_EDGE_VISITED] = BLUE;
    settings_[Setting::COLOR_EDGE_UNVISITED] = LIGHTGRAY;
    settings_[Setting::COLOR_EDGE_CURRENT] = YELLOW;
    settings_[Setting::COLOR_EDGE_PATH] = GREEN;
    settings_[Setting::COLOR_EDGE_BLOCKED] = RED;
    settings_[Setting::COLOR_EDGE_SPECIAL] = PURPLE;

    // algorithm settings
    settings_[Setting::ALGORITHM_IS_AUTO_FORWARD] = true;
    settings_[Setting::ALGORITHM_DROPDOWN_OPTION] = 0;

    // toolbar
    settings_[Setting::TOOLBAR_CURRENT_SELECTED_TOOL] = 0;
    settings_[Setting::TOOLBAR_IS_SELECTING_VERTEX] = true;
    settings_[Setting::TOOLBAR_IS_SELECTING_EDGE] = true;
}

void Application::initFlags(){
    // canvas
    flags_[Flag::TOOLBAR_CAN_UNDO] = false;
    flags_[Flag::TOOLBAR_CAN_REDO] = false;

    // algorithm
    flags_[Flag::ALGORITHM_FOCUS_MODE] = false;
    flags_[Flag::ALGORITHM_IS_RUNNING] = false;
}

template <typename EnumType, typename ValueType>
void Application::setValue(EnumType key, ValueType value) {
    if constexpr(std::is_same_v<EnumType, Setting>){
        if(settings_.find(key) != settings_.end()){
            settings_[key] = value;
        }else{
            throw std::runtime_error("Setting not found!");
        }
    }else if constexpr(std::is_same_v<EnumType, Flag>){
        if(flags_.find(key) != flags_.end()){
            flags_[key] = value;
        }else{
            throw std::runtime_error("Flag not found!");
        }
    }
}

template <typename EnumType, typename ValueType>
ValueType Application::getValue(EnumType key) {
    if constexpr(std::is_same_v<EnumType, Setting>){
        if(settings_.find(key) != settings_.end()){
            return std::get<ValueType>(settings_[key]);
        }else{
            throw std::runtime_error("Setting not found!");
        }
    }else if constexpr(std::is_same_v<EnumType, Flag>){
        if(flags_.find(key) != flags_.end()) {
            return std::get<ValueType>(flags_[key]);
        }else{
            throw std::runtime_error("Flag not found!");
        }
    }
    throw std::runtime_error("Invalid enum type!");
}

template void Application::setValue<Setting, float>(Setting, float);
template void Application::setValue<Setting, int>(Setting, int);
template void Application::setValue<Setting, bool>(Setting, bool);
template void Application::setValue<Setting, Color>(Setting, Color);

template float Application::getValue<Setting, float>(Setting);
template int Application::getValue<Setting, int>(Setting);
template bool Application::getValue<Setting, bool>(Setting);
template Color Application::getValue<Setting, Color>(Setting);

template void Application::setValue<Flag, bool>(Flag, bool);
template bool Application::getValue<Flag, bool>(Flag);

