#include "kruskal.hpp"
#include "system/application.hpp"
#include "graph/graph.hpp"
#include "system/application.hpp"
#include "actions_center/actions_center.hpp"
#include "actions_center/graph_action/dye.hpp"
#include "system/settings.hpp"
#include "system/terminal_prefix.hpp"
#include "actions_center/algorithm/kruskal_add_edge.hpp"

#include <algorithm>
#include <utility>
#include <iostream>

using namespace Algorithm;

Kruskal::Kruskal(){
    printAlgorithmPrefix();
    std::cout << "Kruskal's Algorithm" << std::endl;

    initializeGraphAndMaps();

    for(auto &edge : graph_){
        bool isCurrentEdgeRejected{false};

        if(getParent(edge.startID) != getParent(edge.endID)){
            combine(edge.startID, edge.endID);
        }else{
            isCurrentEdgeRejected = true;
        }

        Application::instance().actionCenter().algorithmDo(
            std::make_unique<Action::KruskalAddEdge>(
                std::make_pair(edge.startID, edge.endID),
                previousEdge_,
                isCurrentEdgeRejected,
                isPreviousRejected_
            )
        );

        previousEdge_ = std::make_pair(edge.startID, edge.endID);
        isPreviousRejected_ = isCurrentEdgeRejected;

        if(isCurrentEdgeRejected) std::cout << " > rejected (";
        else std::cout << " > adding (";
        std::cout << edge.startID << ", " << edge.endID << ")" << std::endl;
        if(!isCurrentEdgeRejected){
            std::cout << " > Combining (" 
                      << edge.startID << ", " << edge.endID << ") -> Roots (" 
                      << getParent(edge.startID) << ", " << getParent(edge.endID) << ")" 
                      << std::endl;
        }
    }
}

void Kruskal::initializeGraphAndMaps(){
    graph_ = Application::instance().graph().getAllNonHiddenEdges();

    std::sort(
        graph_.begin(), graph_.end(),
        [](Normalized::Edge &a, Normalized::Edge &b){
            return a.weight < b.weight;
        }
    );

    for(auto &edge : graph_){
        parentsTable_[edge.startID] = edge.startID;
        parentsTable_[edge.endID] = edge.endID;
        heightsTable_[edge.startID] = 0;
        heightsTable_[edge.endID] = 0;
    }
}

Kruskal::VertexID Kruskal::getParent(VertexID id){
    if(parentsTable_[id] == id) return id;
    return parentsTable_[id] = getParent(parentsTable_[id]);
}

void Kruskal::combine(VertexID id1, VertexID id2){
    VertexID id1Root{getParent(id1)};
    VertexID id2Root{getParent(id2)};

    if(id1Root == id2Root) return;

    if(heightsTable_[id1Root] > heightsTable_[id2Root]){
        parentsTable_[id2Root] = id1Root;
    }else if(heightsTable_[id1Root] < heightsTable_[id2Root]){
        parentsTable_[id1Root] = id2Root;
    }else{
        parentsTable_[id2Root] = id1Root;
        heightsTable_[id1Root]++;
    }
}
