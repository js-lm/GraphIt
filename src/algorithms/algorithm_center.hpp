#pragma once

#include <optional>

class AlgorithmCenter{
public:
    using VertexID = size_t;

public:
    AlgorithmCenter();
    ~AlgorithmCenter() = default;

    void update();

    void run();
    void exit();
    void previousStep();
    void nextStep();

private:
    void executeAlgorithm();
    void replayAlgorithm();

private:
    void setStartVertex();

private:
    std::optional<VertexID> startVertex_;
    std::optional<VertexID> hoveredVertex_;

    bool hasExecuted_;
    float autoClock_;
};
