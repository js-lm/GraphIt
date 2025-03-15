#include "graph_action.h"
#include "../../application.h"
#include "../../graph/graph.h"

#include <raylib.h>
#include <iostream>

using namespace Action;

size_t GraphRelated::addVertex(Vector2 position, std::optional<Color> color){
    std::cout << "GraphRelated::addVertex() called\n";
    return Application::instance().graph().addVertex(position, color);
}

bool GraphRelated::removeVertex(size_t id){
    return Application::instance().graph().removeVertex(id);
}

void GraphRelated::restoreRemovedVertex(size_t id){
    Application::instance().graph().restoreRemovedVertex(id);
}

bool GraphRelated::connectVertices(size_t startID, size_t endID, std::optional<Color> color){
    return Application::instance().graph().connectVertices(startID, endID, color);
}
    
std::optional<Color> GraphRelated::disconnectVertices(size_t startID, size_t endID){
    return Application::instance().graph().disconnectVertices(startID, endID);
}