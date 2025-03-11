#pragma once

class Graph;
class ActionsCenter;
class GUI;

class Application{
public:
    static Application &instance(){
        static Application instance;
        return instance;
    }

    Application(const Application &app) = delete;
    Application& operator=(const Application &app) = delete;

    Graph &graph(){ return *graph_;};
    ActionsCenter &actionCenter(){ return *actionsCenter_;};
    GUI &ui(){ return *ui_;};

    void run();

private:
    Application();
    ~Application();

    Graph *graph_;
    ActionsCenter *actionsCenter_;
    GUI *ui_;
};