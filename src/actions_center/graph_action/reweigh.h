#pragma once

#include "graph_action.h"

namespace Action{

class Reweigh : public GraphRelated{
public:
    Reweigh(
        const std::vector<EdgeID> &edgeIDs,
        float newWeight
    )
        : ids_(edgeIDs)
        , newWeight_(newWeight)
    {
        identifier_ = ID::REWEIGH;
    }

    void execute() override{ 
        oldWeights_ = reweighSelectedEdge(ids_, newWeight_);
    }

    void undo() override{ 
        reweighSelectedEdge(ids_, oldWeights_);
    }

    void redo() override{ 
        reweighSelectedEdge(ids_, newWeight_);
    }

private:
    std::vector<EdgeID> ids_;
    float newWeight_;
    std::vector<float> oldWeights_;
};

} // namespace Action