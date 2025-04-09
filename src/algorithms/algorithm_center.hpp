#pragma once

#include <optional>

// namespace Algorithm{

class AlgorithmCenter{
public:
    AlgorithmCenter() = default;
    ~AlgorithmCenter() = default;

    void update();

    void run();
    // void pause();
    // void resume();
    void exit();
    void previousStep();
    void nextStep();

private:
    // std::optional<float> autoClock_;
    // float autoClock_{0.0f};
};

// } // namespace Algorithm