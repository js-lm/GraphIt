#pragma once

#include "settings_and_flags.h"
#include "ui/ui.h"

#include <variant>
#include <unordered_map>

struct Color;
class Graph;
class ActionsCenter;
class Canvas;
class Serializer;

struct Rectangle;

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
    UI::Center &ui(){ return *ui_;}
    Canvas &canvas(){ return *canvas_;}
    Serializer &serializer(){ return *serializer_;}

public:
    static void initSettings();
    static void initFlags();

    template <typename EnumType, typename ValueType>
    static void setValue(EnumType key, ValueType value);

    template <typename EnumType, typename ValueType>
    static ValueType getValue(EnumType key);

private:
    Application();
    ~Application();

private:
    void init();

    void update();
    void draw();

    void handleWindowResizeEvent();

private:
    Graph *graph_;
    ActionsCenter *actionsCenter_;
    UI::Center *ui_;
    Canvas *canvas_;
    Serializer *serializer_;

    static std::unordered_map<Setting, SettingValue> settings_;
    static std::unordered_map<Flag, SettingValue> flags_;

    // handleWindowResizeEvent()
    Rectangle previousWindowRectangle_;
};