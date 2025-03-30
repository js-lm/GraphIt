#pragma once

#include "ui/ui.hpp"

struct Color;
class Graph;
class ActionsCenter;
class Canvas;
class Serializer;

struct Rectangle;

class Application{
public:
    int run(int argumentsCount, char* arguments[]);
    
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

private:
    Application();
    ~Application();

private:
    bool handleArguments(int argc, char *argv[]);
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

    // handleWindowResizeEvent()
    Rectangle previousWindowRectangle_;
};