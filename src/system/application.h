#pragma once

#include "settings_and_flags.h"

#include <variant>
#include <unordered_map>

struct Color;

class Graph;
class ActionsCenter;
class GUI;
class Canvas;
class Serializer;

class Application{
public:
    using SettingValue = std::variant<int, float, bool, Color>;

public:
    int run();
    
public:
    static Application &instance(){
        static Application instance;
        return instance;
    }

    Application(const Application &app) = delete;
    Application &operator=(const Application &app) = delete;

    Graph &graph(){ return *graph_;}
    ActionsCenter &actionCenter(){ return *actionsCenter_;}
    GUI &ui(){ return *ui_;}
    Canvas &canvas(){ return *canvas_;}
    Serializer &serializer(){ return *serializer_;}

public:
    static void initSettings();
    static void initFlags();

    template <typename EnumType, typename ValueType>
    static void setValue(EnumType key, ValueType value);

    template <typename EnumType, typename ValueType>
    static ValueType getData(EnumType key);

private:
    Application();
    ~Application();

private:
    Graph *graph_;
    ActionsCenter *actionsCenter_;
    GUI *ui_;
    Canvas *canvas_;
    Serializer *serializer_;

    static std::unordered_map<Setting, SettingValue> settings_;
    static std::unordered_map<Flag, SettingValue> flags_;
};