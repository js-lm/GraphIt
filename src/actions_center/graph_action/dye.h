#pragma once

#include "graph_action.h"

namespace Action{

class DyeVertex : public GraphRelated{
public:
    DyeVertex(
        const std::vector<VertexID> &ids,
        Color newColor
    )
        : ids_(ids), newColor_(newColor)
    {
        identifier_ = ID::DYE_VERTEX;
    }

    void execute() override{ oldColor_ = dyeSelectedVertices(ids_, newColor_);}

    void undo() override{ dyeSelectedVertices(ids_, oldColor_);}
    void redo() override{ dyeSelectedVertices(ids_, newColor_);}

private:
    std::vector<VertexID> ids_;
    Color newColor_;
    std::vector<Color> oldColor_;
};

class DyeEdge : public GraphRelated{
public:
    DyeEdge(
        const std::vector<EdgeID> &ids,
        Color newColor
    )
        : ids_(ids), newColor_(newColor)
    {
        identifier_ = ID::DYE_EDGE;
    }

    void execute() override{ oldColor_ = dyeSelectedEdge(ids_, newColor_);}

    void undo() override{ dyeSelectedEdge(ids_, oldColor_);}
    void redo() override{ dyeSelectedEdge(ids_, newColor_);}

private:
    std::vector<EdgeID> ids_;
    Color newColor_;
    std::vector<Color> oldColor_;
};

class Dye : public GraphRelated{
public:
Dye(
        const std::vector<VertexID> &vertexIDs,
        const std::vector<EdgeID> &edgeIDs,
        Color newColor
    )
        : vertexIDs_(vertexIDs), edgeIDs_(edgeIDs)
        , newColor_(newColor)
    {
        identifier_ = ID::DYE;
    }

    void execute() override{ 
        oldVertexColor_ = dyeSelectedVertices(vertexIDs_, newColor_);
        oldEdgeColor_ = dyeSelectedEdge(edgeIDs_, newColor_);
    }

    void undo() override{ 
        dyeSelectedVertices(vertexIDs_, oldVertexColor_);
        dyeSelectedEdge(edgeIDs_, oldEdgeColor_);
    }

    void redo() override{ 
        dyeSelectedVertices(vertexIDs_, newColor_);
        dyeSelectedEdge(edgeIDs_, newColor_);
    }

private:
    std::vector<VertexID> vertexIDs_;
    std::vector<EdgeID> edgeIDs_;
    Color newColor_;
    std::vector<Color> oldVertexColor_;
    std::vector<Color> oldEdgeColor_;
};

} // namespace Action