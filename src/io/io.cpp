#include "io.h"
#include "application.h"
#include "graph/graph.h"
#include "configs/version.h"
#include "configs/terminal_prefix.h"

#include <iostream>
#include <unordered_map>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <stdexcept>
#include <sstream>

Normalized::SaveData Serializer::normalizeData(){
    Normalized::SaveData normalizedGraph;
    Graph &graph{Application::instance().graph()};

    normalizedGraph.graphSettings.isDirected = true;
    normalizedGraph.graphSettings.isWeighted = false;
    
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

    std::vector<std::pair<Graph::EdgeID, Color>> edgeIDsAndColors{graph.getAllValidEdgeIDsAndColor()};

    for(const auto &edgeID : edgeIDsAndColors){
        Normalized::Edge normalizedEdge;

        normalizedEdge.startID = originalIDs[edgeID.first.first];
        normalizedEdge.endID = originalIDs[edgeID.first.second];
        normalizedEdge.weight = 0;
        normalizedEdge.color = edgeID.second;

        normalizedGraph.edges.emplace_back(normalizedEdge);
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

    return true;
}

std::queue<std::string> Serializer::getDataFormat(const std::vector<std::string> &format){
    std::queue<std::string> dataFormat;
    for(const auto &item : format){
        dataFormat.push(item);
    }
    return dataFormat;
}

std::vector<std::string> Serializer::parseData(
    std::string line,
    std::queue<std::string> lineTemplate
){
    if(lineTemplate.empty()){
        printErrorPrefix();
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
Normalized::CameraSettings Serializer::parseCameraSettings(const std::string &data){
    auto dataFormat{getDataFormat(
        {"Position:(", ",", ")__Zoom:", "__|"}
    )};

    auto parsedData{parseData(data, dataFormat)};

    if(parsedData.size() != 3){
        throw std::runtime_error("Data Corrupted: Mismatch Camera Data");
    }

    Normalized::CameraSettings cameraSettings;

    try{
        cameraSettings.position.x = std::stof(parsedData[0]);
        cameraSettings.position.y = std::stof(parsedData[1]);
        cameraSettings.zoom = std::stof(parsedData[2]);
    }catch(const std::invalid_argument &error){
        std::stringstream message;
        message << "Data Corrupted: Abnormal Camera Data ("
                << error.what() << ")";
        throw std::runtime_error(message.str());
    }catch(const std::out_of_range &error){
        std::stringstream message;
        message << "Data Corrupted: Abnormal Camera Data ("
                << error.what() << ")";
        throw std::runtime_error(message.str());
    }

    return cameraSettings;
}

Normalized::GraphSettings Serializer::parseGraphSettings(const std::string &data){
    auto dataFormat{getDataFormat(
        {"Directed:", "__Weighted:", "__|"}
    )};

    auto parsedData{parseData(data, dataFormat)};

    if(parsedData.size() != 2){
        throw std::runtime_error("Data Corrupted: Mismatch Graph Settings Data");
    }

    Normalized::GraphSettings graphSettings;

    try{
        graphSettings.isDirected = parsedData[0] == "1";
        graphSettings.isWeighted = parsedData[1] == "1";
    }catch(const std::exception &error){
        std::stringstream message;
        message << "Data Corrupted: Abnormal Graph Settings Data ("
                << error.what() << ")";
        throw std::runtime_error(message.str());
    }

    return graphSettings;
}

std::pair<size_t, size_t> Serializer::parseMetaData(const std::string &data){
    auto dataFormat{getDataFormat(
        {"V:", "__E:", "__|"}
    )};

    auto parsedData{parseData(data, dataFormat)};

    if(parsedData.size() != 2){
        throw std::runtime_error("Data Corrupted: Mismatch Meta Data");
    }

    std::pair<size_t, size_t> metaData;

    try{
        // std::cout << parsedData[0] << std::endl;
        // std::cout << parsedData[1] << std::endl;
        metaData.first = std::stoull(parsedData[0]);
        metaData.second = std::stoull(parsedData[1]);
    }catch(const std::invalid_argument &error){
        std::stringstream message;
        message << "Data Corrupted: Abnormal Meta Data ("
                << error.what() << ")";
        throw std::runtime_error(message.str());
    }catch(const std::out_of_range &error){
        std::stringstream message;
        message << "Data Corrupted: Abnormal Meta Data ("
                << error.what() << ")";
        throw std::runtime_error(message.str());
    }

    return metaData;
}

std::vector<Normalized::Vertex> Serializer::parseVertexData(const std::vector<std::string> &data){
    auto dataFormat{getDataFormat(
        {"ID:", "__Position:(", ",", ")__Color:{R:", ",G:", ",B:", ",A:", "}__|"}
    )};

    std::vector<Normalized::Vertex> vertices;
    
    for(const auto &line : data){
        auto parsedData{parseData(line, dataFormat)};

        if(parsedData.size() != 7){
            throw std::runtime_error("Data Corrupted: Mismatch Vertex Data");
        }

        Normalized::Vertex vertex;

        try{
            vertex.id = std::stoull(parsedData[0]);
            vertex.position.x = std::stof(parsedData[1]);
            vertex.position.y = std::stof(parsedData[2]);
            vertex.color.r = std::stoi(parsedData[3]);
            vertex.color.g = std::stoi(parsedData[4]);
            vertex.color.b = std::stoi(parsedData[5]);
            vertex.color.a = std::stoi(parsedData[6]);
        }catch(const std::invalid_argument &error){
            std::stringstream message;
            message << "Data Corrupted: Abnormal Vertex Data ("
                    << error.what() << ")";
            throw std::runtime_error(message.str());
        }catch(const std::out_of_range &error){
            std::stringstream message;
            message << "Data Corrupted: Abnormal Vertex Data ("
                    << error.what() << ")";
            throw std::runtime_error(message.str());
        }

        vertices.emplace_back(vertex);
    }

    return vertices;
}

std::vector<Normalized::Edge> Serializer::parseEdgeData(const std::vector<std::string> &data){
    auto dataFormat{getDataFormat(
        {"StartID:", "__EndID:", "__Weight:", "__Color:{R:", ",G:", ",B:", ",A:", "}__|"}
    )};

    std::vector<Normalized::Edge> edges;
    
    for(const auto &line : data){
        auto parsedData{parseData(line, dataFormat)};

        if(parsedData.size() != 7){
            throw std::runtime_error("Data Corrupted: Mismatch Edge Data");
        }

        Normalized::Edge edge;

        try{
            // for(size_t i{0}; i < 7; i++){
            //     std::cout << "[" << i << "]" << parsedData[i] << std::endl;
            // }
            edge.startID = std::stoull(parsedData[0]);
            edge.endID = std::stoull(parsedData[1]);
            edge.weight = std::stof(parsedData[2]);
            edge.color.r = std::stoi(parsedData[3]);
            edge.color.g = std::stoi(parsedData[4]);
            edge.color.b = std::stoi(parsedData[5]);
            edge.color.a = std::stoi(parsedData[6]);
        }catch(const std::invalid_argument &error){
            std::stringstream message;
            message << "Data Corrupted: Abnormal Edge Data ("
                    << error.what() << ")";
            throw std::runtime_error(message.str());
        }catch(const std::out_of_range &error){
            std::stringstream message;
            message << "Data Corrupted: Abnormal Edge Data ("
                    << error.what() << ")";
            throw std::runtime_error(message.str());
        }

        edges.emplace_back(edge);
    }

    return edges;
}