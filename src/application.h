#pragma once

class Graph;
class ActionsCenter;
class GUI;
class Canvas;
class Serializer;

class Application{
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

private:
    Application();
    ~Application();

private:
    Graph *graph_;
    ActionsCenter *actionsCenter_;
    GUI *ui_;
    Canvas *canvas_;
    Serializer *serializer_;
};