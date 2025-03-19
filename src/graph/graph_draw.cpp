#include "graph.h"
#include "application.h"
#include "canvas/canvas.h"

#include <raylib.h>

void Graph::draw() const{
    drawEdges();
    drawVertices();
}

void Graph::drawVertices() const{
    auto &hoveredVertexID{Application::instance().canvas().getHoveredVertexID()};
    auto &selectedVertexIDs{Application::instance().canvas().getSelectedVertexIDs()};

    for(const auto &vertex : vertices_){
        if(vertex->isHidden()) continue;
        
        float vertexRadius{vertexRadius_};
        if(hoveredVertexID == vertex->id()) vertexRadius *= 1.5f;

        DrawCircleV(vertex->position(), vertexRadius, vertex->color());

        if(selectedVertexIDs.find(vertex->id()) != selectedVertexIDs.end()){ 
            // DrawCircleLinesV(vertex->position(), vertexRadius * 1.5f, vertex->color());
            DrawCircleV(vertex->position(), vertexRadius * 2.0f, Fade(vertex->color(), .5f));
        } 
    }
}

void Graph::drawEdges() const{
    auto &hoveredEdgeIDs{Application::instance().canvas().getHoveredEdgeIDs()};
    auto &selectedEdgeIDs{Application::instance().canvas().getSelectedEdgeIDs()};

    for(const auto &edge : edges_){
        if(isVertexHidden(edge->startID())
        || isVertexHidden(edge->endID())
        ){
            continue;
        }
        
        float edgeThickness{edgeThickness_};
        if(hoveredEdgeIDs == std::pair{edge->startID(), edge->endID()}){
            edgeThickness *= 1.25f;
        }

        DrawLineEx(
            vertices_[edge->startID()]->position(), 
            vertices_[edge->endID()]->position(), 
            edgeThickness, 
            edge->color()
        );
        
        if(selectedEdgeIDs.find(
            std::pair{edge->startID(), edge->endID()}
           ) != selectedEdgeIDs.end()
        ){
            DrawLineEx(
                vertices_[edge->startID()]->position(), 
                vertices_[edge->endID()]->position(), 
                edgeThickness_ * 2.0f, 
                Fade(edge->color(), .5f)
            );
        }
    }
}