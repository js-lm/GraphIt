#include "dijkstra.hpp"
#include "system/application.hpp"
#include "actions_center/actions_center.hpp"
#include "system/settings.hpp"
#include "actions_center/graph_action/dye.hpp"
#include "graph/graph.hpp"
#include "actions_center/algorithm/visit_vertex.hpp"
#include "actions_center/algorithm/switch_current_vertex.hpp"
#include "system/terminal_prefix.hpp"

#include <iostream>
#include <raylib.h>
#include <algorithm>

using namespace Algorithm;

Dijkstra::Dijkstra(std::optional<VertexID> startVertex)
    : startVertex_(startVertex)
{
    appSettings.graphIsWeighted = true;
    if(startVertex_) run();
}

void Dijkstra::run(){
    printAlgorithmPrefix();
    std::cout << "Dijkstra's Algorithm" << std::endl;

    auto allVertexIDs{Application::instance().graph().getAllNonHiddenVertexIDs()};
    for(const auto &vertexID : allVertexIDs){
        distances_[vertexID] = std::numeric_limits<float>::infinity();
    }

    distances_[startVertex_.value()] = .0f;
    pivotVertex_ = startVertex_.value();
    previousVertex_ = startVertex_.value();
    
    priorityQueue_.push({startVertex_.value(), .0f});

    while(!priorityQueue_.empty()){
        VertexID currentVertex{priorityQueue_.top().vertexID};
        priorityQueue_.pop();
        
        if(hasVisited(currentVertex)) continue;

        visitedVertices_.push_back(currentVertex);
        
        if(currentVertex != startVertex_.value()){
            VertexID predecessor{predecessors_[currentVertex]};
            visitedEdges_.emplace_back(std::make_pair(predecessor, currentVertex));
        }

        visit(currentVertex, distances_[currentVertex]);

        std::cout << " > Processing vertex " << currentVertex 
                  << " (distance: " << distances_[currentVertex] << ")" << std::endl;

        previousVertex_ = pivotVertex_;
        pivotVertex_ = currentVertex;

        std::vector<AdjacentVertex> adjacentVertices{retrieveAdjacentVertices(currentVertex)};

        for(const auto &adjacentVertex : adjacentVertices){
            if(hasVisited(adjacentVertex.id)) continue;

            float newDistance{distances_[currentVertex] + adjacentVertex.weight};
            
            std::cout << "   > Relaxing edge to vertex " << adjacentVertex.id 
                      << " (weight: " << adjacentVertex.weight 
                      << ", current dist: " << distances_[adjacentVertex.id]
                      << ", new dist: " << newDistance << ")" << std::endl;

            if(newDistance < distances_[adjacentVertex.id]){
                distances_[adjacentVertex.id] = newDistance;
                predecessors_[adjacentVertex.id] = currentVertex;
                priorityQueue_.push({adjacentVertex.id, newDistance});
                
                std::cout << "   > Updated distance to " << adjacentVertex.id 
                          << ": " << newDistance << std::endl;
            }
        }
    }

    std::cout << "\nShortest distances from vertex " << startVertex_.value() << ":" << std::endl;
    for(const auto &[vertexID, distance] : distances_){
        if(distance != std::numeric_limits<float>::infinity()){
            std::cout << " > Vertex " << vertexID << ": " 
                      << distance << std::endl;
        }
    }
}

void Dijkstra::visit(VertexID vertexID, float distance){
    EdgeID edgeToVisit;
    
    if(vertexID == startVertex_.value()){
        edgeToVisit = std::make_pair(vertexID, vertexID);
    }else{
        edgeToVisit = std::make_pair(predecessors_[vertexID], vertexID);
    }

    Application::instance().actionCenter().algorithmDo(
        std::make_unique<Algorithm::VisitVertex>(
            vertexID, 
            edgeToVisit,
            pivotVertex_,
            visitedVertices_,
            visitedEdges_
        )
    );
}

bool Dijkstra::hasVisited(VertexID vertexID){
    return std::find(visitedVertices_.begin(), visitedVertices_.end(), vertexID) != visitedVertices_.end();
}

std::vector<AdjacentVertex> Dijkstra::retrieveAdjacentVertices(VertexID vertexID){
    auto retrievedAdjacentVertices{Application::instance().graph().retrieveAdjacentVertices(vertexID)};
    std::vector<AdjacentVertex> adjacentVertices{};
    for(const auto &vertex : retrievedAdjacentVertices){
        adjacentVertices.emplace_back(AdjacentVertex{vertex.first, vertex.second});
    }
    return adjacentVertices;
}
