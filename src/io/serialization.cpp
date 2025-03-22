#include "io.h"
#include "configs/terminal_prefix.h"

#include <iostream>
#include <unordered_map>
#include <stdexcept>
#include <sstream>

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