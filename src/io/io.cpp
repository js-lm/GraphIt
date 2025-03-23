#include "io.h"
#include "application.h"
#include "graph/graph.h"
#include "configs/version.h"
#include "configs/terminal_prefix.h"
#include "canvas/canvas.h"

#include <iostream>
#include <unordered_map>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <stdexcept>
#include <raymath.h>

Normalized::SaveData Serializer::normalizeData(){
    Normalized::SaveData normalizedGraph;
    Graph &graph{Application::instance().graph()};
    Canvas &canvas{Application::instance().canvas()};

    normalizedGraph.cameraSettings.position = canvas.getCameraPosition();
    normalizedGraph.cameraSettings.zoom = canvas.getCameraZoom();

    normalizedGraph.graphSettings.isDirected = graph.isDirected();
    normalizedGraph.graphSettings.isWeighted = graph.isWeighted();
    
    // index: new ids
    // element: original ids
    const auto &vertexIDs{graph.getAllValidVertexIDs()};
    std::unordered_map<size_t, size_t> originalIDs;

    for(size_t i{0}; i < vertexIDs.size(); i++){
        Normalized::Vertex normalizedVertex;

        normalizedVertex.id = i;
        normalizedVertex.position = graph.getVertexPosition(vertexIDs[i]);
        normalizedVertex.color = graph.getVertexColor(vertexIDs[i]);

        originalIDs[vertexIDs[i]] = i;

        normalizedGraph.vertices.emplace_back(normalizedVertex);
    }

    std::vector<Normalized::Edge> edges{graph.getAllValidEdges()};

    for(auto &edge : edges){
        edge.startID = originalIDs[edge.startID];
        edge.endID = originalIDs[edge.endID];

        normalizedGraph.edges.emplace_back(edge);
    }

    return normalizedGraph;
}

bool Serializer::save(const std::string &name){
    const std::string filename{name.ends_with(".grt") ? name : name + ".grt"};

    std::ofstream saveFile{filename};
    if(!saveFile.is_open()){
        printErrorPrefix();
        std::cerr << "Unable to create " << filename << std::endl;
        return false;
    }     

    auto graph{normalizeData()};

    if(graph.vertices.size() + graph.edges.size() == 0){
        printErrorPrefix();
        std::cerr << "Nothing to save" << std::endl;
        return false;
    }

    time_t rawTime{std::time(nullptr)};
    auto timeInfo{*std::localtime(&rawTime)};

    saveFile << "GraphIt! v" << GRAPHIT_VERSION_STRING << "\n"
             << "Created at " << std::put_time(&timeInfo, "%Y-%m-%d %H:%M:%S") << "\n";

    // camera - line 3
    // Position:(000.00,000.00)__Zoom:0.00__|
    saveFile << "Position:(" << graph.cameraSettings.position.x 
                      << "," << graph.cameraSettings.position.y << ")__"
             << "Zoom:" << std::setprecision(3) << graph.cameraSettings.zoom << "__|\n";

    // graph settings - line 4
    // Directed:0__Weighted:0__|
    saveFile << "Directed:" << graph.graphSettings.isDirected 
           << "__Weighted:" << graph.graphSettings.isWeighted << "__|\n";

    // vertex and edge count - line 5
    // V:0__E:0__|
    saveFile << "V:" << graph.vertices.size()
           << "__E:" << graph.edges.size() << "__|\n";

    for(const auto &vertex : graph.vertices){
        saveFile << "ID:" << vertex.id << "__"
                 << std::fixed << std::setprecision(2)
                 << "Position:(" << vertex.position.x << "," << vertex.position.y << ")__"
                 << "Color:{R:" << static_cast<int>(vertex.color.r)
                       << ",G:" << static_cast<int>(vertex.color.g)
                       << ",B:" << static_cast<int>(vertex.color.b)
                       << ",A:" << static_cast<int>(vertex.color.a)
                       << "}__|" << "\n";
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
                       << "}__|" << "\n";
    }

    saveFile.close();

    return true;
}

std::vector<std::string> Serializer::splitLines(std::ifstream &file){
    std::vector<std::string> lines;
    std::string line;
    while(std::getline(file, line)){
        lines.push_back(line);
    }
    return lines;
}

bool Serializer::load(const std::string &filename){
    std::ifstream loadFile(filename);

    if(!loadFile.is_open()){
        printErrorPrefix();
        std::cerr << "Unable to open " << filename << std::endl;
        return false;
    }

    std::vector<std::string> loadedData{splitLines(loadFile)};

    if(loadedData.size() < 5){
        printErrorPrefix();
        std::cerr << "File is corrupted or missing data" << std::endl;
        return false;
    }

    loadFile.close();

    const std::string &metaData{loadedData[4]};

    // read the meta data
    size_t vertexCount{0};
    size_t edgeCount{0};
    try{
        auto parsedMetaData{parseMetaData(metaData)};

        vertexCount = parsedMetaData.first;
        edgeCount = parsedMetaData.second;
    }catch(const std::runtime_error &error){
        printErrorPrefix();
        std::cerr << error.what() << std::endl;
        return false;
    }

    // read the rest
    auto vertexDataStart{loadedData.begin() + 5};
    auto vertexDataEnd{vertexDataStart + vertexCount};
    auto edgeDataStart{vertexDataEnd};
    auto edgeDataEnd{edgeDataStart + edgeCount};

    std::string cameraData{loadedData[2]};
    std::string graphSettings{loadedData[3]};
    std::vector<std::string> vertexData{vertexDataStart, vertexDataEnd};
    std::vector<std::string> edgeData{edgeDataStart, edgeDataEnd};

    Normalized::SaveData saveData;

    try{
        saveData.cameraSettings = parseCameraSettings(cameraData);
        saveData.graphSettings = parseGraphSettings(graphSettings);
        saveData.vertices = parseVertexData(vertexData);
        saveData.edges = parseEdgeData(edgeData);

    }catch(const std::runtime_error &error){
        printErrorPrefix();
        std::cerr << error.what()  << std::endl;
        return false;
    }

    Application::instance().graph().loadNewGraph(saveData);

    Canvas &canvas{Application::instance().canvas()};
    canvas.setCameraPosition(saveData.cameraSettings.position);
    canvas.setCameraZoom(Clamp(saveData.cameraSettings.zoom, .2f, 2.0f));

    return true;
}