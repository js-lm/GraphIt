#include "canvas.h"
#include "../application.h"
#include "../graph/graph.h"
#include "../actions_center/actions_center.h"
#include "../actions_center/graph_action/graph_action.h"
#include "lib/magic_enum.hpp"

#include <optional>
#include <iostream>

void Canvas::updateMouseActions(){
    switch(mode_){
    case Mode::VIEW: break;
    case Mode::SELECT: break;
    case Mode::MOVE: break;
    case Mode::PEN: updatePen(); break;
    case Mode::LINK: updateLink(); break;  
    case Mode::DRAG: break;
    case Mode::ERASER: updateEraser(); break;
    default: break;
    }
}

void Canvas::updateKeyboardActions(){
    // this should be moved to GUI instead of canvas
    if(IsKeyDown(KEY_LEFT_CONTROL)){
        if(IsKeyDown(KEY_LEFT_SHIFT)){
            if(IsKeyPressed(KEY_Z)){
                Application::instance().actionCenter().redo();
            }
        }else{
            if(IsKeyPressed(KEY_Z)){
                Application::instance().actionCenter().undo();
            }
        }
    }

    auto changeMode{
        [&](Mode mode){
            mode_ = mode;
            std::cout << "Current Mode: " << magic_enum::enum_name(mode_) << std::endl;
        }
    };

    if(IsKeyPressed(KEY_P)) changeMode(Mode::PEN);
    if(IsKeyPressed(KEY_L)) changeMode(Mode::LINK);
    if(IsKeyPressed(KEY_E)) changeMode(Mode::ERASER);
}

void Canvas::resetToolStatus(){
    linkFrom_ = std::nullopt;
    hoveredVertexID_ = std::nullopt;
    hoveredEdgeIDs_ = std::nullopt;
    selectedVertexIDs_.clear();
    selectedEdgeIDs_.clear();
}

void Canvas::updatePen(){
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        Application::instance().actionCenter().addAction(
            std::make_unique<Action::AddVertex>(
                getMousePositionInCanvas(),
                BLACK
            )
        );
    }
}

void Canvas::updateLink(){
    if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) linkFrom_ = std::nullopt;
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        if(!linkFrom_){
            linkFrom_ = hoveredVertexID_;

        }else if(hoveredVertexID_){
            Application::instance().actionCenter().addAction(
                std::make_unique<Action::ConnectVertices>(
                    linkFrom_.value(), 
                    hoveredVertexID_.value()
                )
            );

            linkFrom_ = hoveredVertexID_;
        }
    }
}

void Canvas::updateEraser(){
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        if(hoveredVertexID_){
            Application::instance().actionCenter().addAction(
                std::make_unique<Action::RemoveVertex>(
                    hoveredVertexID_.value()
                )
            );
        }else if(hoveredEdgeIDs_){
            Application::instance().actionCenter().addAction(
                std::make_unique<Action::DisconnectVertices>(
                    hoveredEdgeIDs_.value().first,
                    hoveredEdgeIDs_.value().second
                )
            );
        }
    }
}