#include "bfs.hpp"
#include "system/application.hpp"
#include "actions_center/actions_center.hpp"
#include "system/settings.hpp"
#include "actions_center/graph_action/dye.hpp"
#include "graph/graph.hpp"
#include "actions_center/algorithm/visit_vertex.hpp"
#include "actions_center/algorithm/switch_current_vertex.hpp"

#include <iostream>
#include <raylib.h>
#include <algorithm>

using namespace Algorithm;

void BFS::run(){
    previousVertex_ = startVertex_;
    visit(startVertex_);

    while(!bfsQueue_.empty()){
        switchCurrentVertex();
        std::vector<AdjacentVertex> adjacentVertices{retrieveAdjacentVertices(pivotVertex_)};

        for(const auto &adjacentVertex : adjacentVertices){
            if(!hasVisited(adjacentVertex.id)){
                visit(adjacentVertex.id);
            }

            std::cout << "\t\t> id:" << adjacentVertex.id
                      << " weight:" << adjacentVertex.weight
                      << std::endl;
        }
    }
}

void BFS::visit(VertexID id){
    visitedVertices_.emplace_back(id);
    bfsQueue_.push(id);

    visitedEdge_.emplace_back(std::make_pair(pivotVertex_, id));

    Application::instance().actionCenter().algorithmDo(
        std::make_unique<Algorithm::VisitVertex>(
            id, 
            std::make_pair(previousVertex_, id),
            pivotVertex_,
            visitedVertices_,
            visitedEdge_
        )
    );
}

bool BFS::hasVisited(VertexID id){
    return std::find(visitedVertices_.begin(), visitedVertices_.end(), id) != visitedVertices_.end();
}

void BFS::switchCurrentVertex(){
    pivotVertex_ = bfsQueue_.front();
    bfsQueue_.pop();

    Application::instance().actionCenter().algorithmDo(
        std::make_unique<Algorithm::SwitchCurrentVertex>(
            pivotVertex_, previousVertex_,
            visitedVertices_,
            visitedEdge_
        )
    );

    previousVertex_ = pivotVertex_;
}

std::vector<AdjacentVertex> BFS::retrieveAdjacentVertices(VertexID id){
    auto retrievedAdjacentVertices{Application::instance().graph().retrieveAdjacentVertices(id)};
    std::vector<AdjacentVertex> adjacentVertices;
    for(const auto &vertex : retrievedAdjacentVertices){
        adjacentVertices.emplace_back(vertex.first, vertex.second);
    }
    return adjacentVertices;
}
