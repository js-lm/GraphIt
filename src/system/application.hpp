#pragma once

#include "ui/ui.hpp"
// #include "algorithms/algorithm_center.hpp"

struct Color;
class Graph;
class ActionsCenter;
class Canvas;
class Serializer;
class AlgorithmCenter;

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
    AlgorithmCenter &algorithm(){ return *algorithm_;}

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
    AlgorithmCenter *algorithm_;

    // handleWindowResizeEvent()
    Rectangle previousWindowRectangle_;
};