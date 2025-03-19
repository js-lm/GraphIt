#include "io.h"
#include "application.h"
#include "graph/graph.h"
#include "configs/version.h"

#include <iostream>
#include <unordered_map>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <stdexcept>

IO::SaveData Serializer::normalizeData(){
    IO::SaveData normalizedGraph;
    Graph &graph{Application::instance().graph()};

    normalizedGraph.isDirected = true;
    normalizedGraph.isWeighted = false;
    
    // index: new ids
    // element: original ids
    std::vector<Graph::VertexID> vertexIDs{graph.getAllValidVertexIDs()};
    std::unordered_map<size_t, size_t> originalIDs;

    for(size_t i{0}; i < vertexIDs.size(); i++){
        IO::NormalizedVertex normalizedVertex;

        normalizedVertex.id = i;
        normalizedVertex.position = graph.getVertexPosition(vertexIDs[i]);
        normalizedVertex.color = graph.getVertexColor(vertexIDs[i]);

        originalIDs[vertexIDs[i]] = i;

        normalizedGraph.vertices.emplace_back(normalizedVertex);
    }

    std::vector<std::pair<Graph::EdgeID, Color>> edgeIDsAndColors{graph.getAllValidEdgeIDsAndColor()};

    for(const auto &edgeID : edgeIDsAndColors){
        IO::NormalizedEdge normalizedEdge;

        normalizedEdge.startID = originalIDs[edgeID.first.first];
        normalizedEdge.endID = originalIDs[edgeID.first.second];
        normalizedEdge.weight = 0;
        normalizedEdge.color = edgeID.second;

        normalizedGraph.edges.emplace_back(normalizedEdge);
    }

    return normalizedGraph;
}

bool Serializer::save(const std::string &name){
    std::string filename{name};
    if(!filename.ends_with(".grt")) filename += ".grt";

    std::ofstream saveFile{filename};
    if(!saveFile.is_open()){
        std::cerr << "Unable to create " << filename << std::endl;
        return false;
    }     

    auto graph{normalizeData()};

    if(graph.vertices.size() + graph.edges.size() == 0){
        std::cerr << "Nothing to save" << std::endl;
        return false;
    }

    time_t rawTime{std::time(nullptr)};
    auto timeInfo{*std::localtime(&rawTime)};

    saveFile << "GraphIt v" << GRAPHIT_VERSION_STRING << "\n"
             << "Created at " << std::put_time(&timeInfo, "%Y-%m-%d %H:%M:%S") << "\n"
             // camera
             << "Position:(" << "0" << "," << "0" << ")__"
             << "Zoom:" << std::setprecision(3) << 1.62342324f << "\n"
             // graph settings
             << "Directed:" << graph.isDirected << "__Weighted:" << graph.isWeighted << "\n"
             // vertex and edge count
             << "V:" << graph.vertices.size() << "__E:" << graph.edges.size() << "\n";

    for(const auto &vertex : graph.vertices){
        saveFile << "ID:" << vertex.id << "__"
                 << std::fixed << std::setprecision(2)
                 << "Position:(" << vertex.position.x << "," << vertex.position.y << ")__"
                 << "Color:{R:" << static_cast<int>(vertex.color.r)
                       << ",G:" << static_cast<int>(vertex.color.g)
                       << ",B:" << static_cast<int>(vertex.color.b)
                       << ",A:" << static_cast<int>(vertex.color.a)
                       << "} " << "\n";
    }

    for(const auto &edge : graph.edges){
        saveFile << "StartID:" << edge.startID << "__"
                 << "EndID:" << edge.endID << "__"
                 << std::fixed << std::setprecision(2)
                 << "Weight:" << edge.weight << "__"
                 << "Color:{R:" << static_cast<int>(edge.color.r)
                       << ",G:" << static_cast<int>(edge.color.g)
                       << ",B:" << static_cast<int>(edge.color.b)
                       << ",A:" << static_cast<int>(edge.color.a)
                       << "} " << "\n";
    }

    saveFile.close();

    return true;
}

bool Serializer::load(const std::string &filename){



    std::vector<std::string> data{splitLines(file)};

    std::string cameraData{data[2]};
    std::string graphSettings{data[3]};
    std::string graphSize{data[4]};



    return true;
}

std::vector<std::string> Serializer::parseData(
    const std::string &line,
    std::queue<std::string> lineTemplate
){
    if(lineTemplate.empty()){
        std::cerr << "Template is empty" << std::endl;
    }

    std::vector<std::string> data;

    for(size_t lastDataStringLength{0}; lineTemplate.size() > 1; lastDataStringLength = data.back().size()){
        size_t metaStringLength{lineTemplate.front().size() + lastDataStringLength};
        line.erase(0, metaStringLength);
        lineTemplate.pop();
    
        data.emplace_back(line.substr(0, line.find(lineTemplate.front())));
    }

    return data;
}

std::vector<IO::NormalizedVertex> Serializer::parseVertexData(const std::vector<std::string> &data){
    std::queue<std::string> dateFormat;
    for(const auto &item : {"ID:", "__Position:(", ",", ")__Color:{R:", ",G:", ",B:", ",A:", "}"}){
        dateFormat.push(item);
    }

    std::vector<IO::NormalizedVertex> vertices;
    
    for(const auto &line : data){
        auto data{parseData(line, dateFormat)};

        if(data.size() != 7){
            throw std::runtime_error("Data Corrupted: Missing Some Vertex Data");
        }

        IO::NormalizedVertex vertex;

        try{
            vertex.id = std::stoull(data[0]);
            vertex.position.x = std::stof(data[1]);
            vertex.position.y = std::stof(data[2]);
            vertex.color.r = std::stoi(data[3]);
            vertex.color.g = std::stoi(data[4]);
            vertex.color.b = std::stoi(data[5]);
            vertex.color.a = std::stoi(data[6]);
        }catch(const std::invalid_argument &error){
            std::stringstream message;
            message << "Data Corrupted: Abnormal Vertex Data ("
                    << error.what() << ")";
            throw std::runtime_error(message);
        }catch(const std::out_of_range &error){
            std::stringstream message;
            message << "Data Corrupted: Abnormal Vertex Data ("
                    << error.what() << ")";
            throw std::runtime_error(message);
        }

        vertices.emplace_back(vertex);
    }

    return vertices;
}