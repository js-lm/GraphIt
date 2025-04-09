#pragma once

#include "graph/vertex_and_edge.hpp"

#include <raylib.h>
#include <vector>
#include <utility>
#include <string>
#include <queue>

namespace Normalized{

    struct CameraSettings{
        Vector2 position;
        float zoom;
    };

    struct GraphSettings{
        bool isDirected;
        bool isWeighted;
        bool isLabeled;
    };

    struct Vertex{
        size_t id;
        Vector2 position;
        Color color;
    };

    struct Edge{
        size_t startID;
        size_t endID;
        float weight;
        Color color;
    };

    struct SaveData{
        CameraSettings cameraSettings;
        GraphSettings graphSettings;
        std::vector<Vertex> vertices;
        std::vector<Edge> edges;
    };

} // namespace Normalized

class Serializer{
public:
    Serializer() = default;
    ~Serializer() = default;

    static bool save(const std::string &filename);
    static bool load(const std::string &filename);

private:
    static Normalized::SaveData normalizeData();

    static std::vector<std::string> parseData(
        std::string line,
        std::queue<std::string> lineTemplate
    );

    static Normalized::CameraSettings parseCameraSettings(const std::string &data);
    static Normalized::GraphSettings parseGraphSettings(const std::string &data);
    static std::pair<size_t, size_t> parseMetaData(const std::string &data);
    
    static std::vector<Normalized::Vertex> parseVertexData(const std::vector<std::string> &data);
    static std::vector<Normalized::Edge> parseEdgeData(const std::vector<std::string> &data);

private:
    static std::queue<std::string> getDataFormat(const std::vector<std::string> &format);

    static std::vector<std::string> splitLines(std::ifstream &file);
};