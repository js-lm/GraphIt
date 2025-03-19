#pragma once

#include "graph/vertex_and_edge.h"

#include <raylib.h>
#include <vector>
#include <utility>
#include <string>
#include <queue>

namespace IO{

    struct NormalizedVertex{
        size_t id;
        Vector2 position;
        Color color;
    };

    struct NormalizedEdge{
        size_t startID;
        size_t endID;
        float weight;
        Color color;
    };

    struct SaveData{
        bool isDirected;
        bool isWeighted;
        std::vector<NormalizedVertex> vertices;
        std::vector<NormalizedEdge> edges;
    };

} // namespace IO

class Serializer{
public:
    Serializer() = default;
    ~Serializer() = default;

    static bool save(const std::string &filename);
    static bool load(const std::string &filename);

private:
    static IO::SaveData normalizeData();

    static std::vector<std::string> parseData(
        const std::string &line,
        std::queue<std::string> lineTemplate
    );

    static std::vector<IO::NormalizedVertex> parseVertexData(const std::vector<std::string> &data);

private:
    static constexpr std::queue<std::string> VERTEX_DATA_FORMAT;
};