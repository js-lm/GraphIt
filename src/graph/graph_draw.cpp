#include "graph.hpp"
#include "system/application.hpp"
#include "canvas/canvas.hpp"

#include <raylib.h>
#include <raymath.h>
#include <iomanip>
#include <sstream>

void Graph::draw() const{
    drawEdges();
    drawVertices();
    drawLabels();
    drawWeights();
}

void Graph::drawVertices() const{
    auto &hoveredVertexID{Application::instance().canvas().getHoveredVertexID()};
    auto &selectedVertexIDs{Application::instance().canvas().getSelectedVertexIDs()};

    float vertexRadius{Application::getValue<Setting, float>(Setting::GRAPH_VERTEX_RADIUS)};

    for(const auto &vertex : vertices_){
        if(vertex->isHidden()) continue;
        
        float vertexRadiusForThisVertex{vertexRadius};
        if(hoveredVertexID == vertex->id()) vertexRadiusForThisVertex *= 1.2f;

        // highlight aura
        if(selectedVertexIDs.find(vertex->id()) != selectedVertexIDs.end()){ 
            DrawCircleV(vertex->position(), vertexRadiusForThisVertex * 1.5f, Fade(vertex->color(), .5f));
        }

        // main circle
        DrawCircleV(vertex->position(), vertexRadiusForThisVertex, vertex->color());
        DrawCircleV(vertex->position(), vertexRadiusForThisVertex * .6f, WHITE);
    }
}

void Graph::drawEdges() const{
    auto &hoveredEdgeIDs{Application::instance().canvas().getHoveredEdgeIDs()};
    auto &selectedEdgeIDs{Application::instance().canvas().getSelectedEdgeIDs()};

    float edgeThickness{Application::getValue<Setting, float>(Setting::GRAPH_EDGE_THICKNESS)};

    for(const auto &edge : edges_){
        if(isVertexHidden(edge->startID())
        || isVertexHidden(edge->endID())
        ){
            continue;
        }
        
        float edgeThicknessForThisEdge{edgeThickness};
        if(hoveredEdgeIDs == std::pair{edge->startID(), edge->endID()}){
            edgeThicknessForThisEdge *= 1.25f;
        }

        const Vector2 &startPoint{vertices_[edge->startID()]->position()};
        const Vector2 &endPoint{vertices_[edge->endID()]->position()};

        // highlight aura
        if(selectedEdgeIDs.find(
            std::pair{edge->startID(), edge->endID()}
            ) != selectedEdgeIDs.end()
        ){
            DrawLineEx(
                startPoint, 
                endPoint, 
                edgeThicknessForThisEdge * 2.0f, 
                Fade(edge->color(), .5f)
            );

            drawArrow(
                startPoint, 
                endPoint, 
                edgeThicknessForThisEdge * 1.5f, 
                Fade(edge->color(), .5f)
            );
        }

        DrawLineEx(startPoint, endPoint, edgeThicknessForThisEdge, edge->color());

        drawArrow(
            startPoint, 
            endPoint, 
            edgeThicknessForThisEdge, 
            edge->color()
        );
    }
}

void Graph::drawArrow(const Vector2 &startPoint, const Vector2 &endPoint, float thickness, Color color) const{
    if(!Application::getValue<Setting, bool>(Setting::GRAPH_IS_DIRECTED)) return;

    float vertexRadius{Application::getValue<Setting, float>(Setting::GRAPH_VERTEX_RADIUS)};

    Vector2 arrowVector{Vector2Subtract(endPoint, startPoint)};

    Vector2 arrowDirection{Vector2Normalize(arrowVector)};
    float originalDistance{Vector2Distance(startPoint, endPoint)};

    Vector2 lineVector{arrowDirection * (originalDistance - vertexRadius)};

    Vector2 arrowEndPoint{Vector2Add(startPoint, lineVector)};

    Vector2 direction{Vector2Normalize(Vector2Subtract(arrowEndPoint, startPoint))};
    Vector2 rotateAngel1{Vector2Rotate(direction, 70.0f * DEG2RAD)};
    Vector2 rotateAngel2{Vector2Rotate(direction, 110.0f * DEG2RAD)};

    Vector2 arrow1{Vector2Add(arrowEndPoint, Vector2Scale(Vector2Add(direction, rotateAngel1), thickness * -2.0f))};
    Vector2 arrow2{Vector2Add(arrowEndPoint, Vector2Scale(Vector2Subtract(direction, rotateAngel2), thickness * -2.0f))};

    DrawLineEx(arrowEndPoint, arrow1, thickness, color);
    DrawLineEx(arrowEndPoint, arrow2, thickness, color);
}

void Graph::drawLabels() const{
    if(!Application::getValue<Setting, bool>(Setting::GRAPH_IS_LABELED)) return;

    auto &hoveredVertexID{Application::instance().canvas().getHoveredVertexID()};

    float vertexRadius{Application::getValue<Setting, float>(Setting::GRAPH_VERTEX_RADIUS)};

    for(const auto &vertex : vertices_){
        if(vertex->isHidden()) continue;
        
        float vertexRadiusForThisVertex{vertexRadius};
        if(hoveredVertexID == vertex->id()) vertexRadiusForThisVertex *= 1.2f;

        // label
        DrawText(
            getLabel(vertex->id()).c_str(), 
            vertex->x() + vertexRadiusForThisVertex / 1.5f,
            vertex->y() + vertexRadiusForThisVertex / 1.5f,
            vertexRadiusForThisVertex, 
            vertex->color()
        );
    }
}

void Graph::drawWeights() const{
    if(!Application::getValue<Setting, bool>(Setting::GRAPH_IS_WEIGHTED)) return;

    auto &hoveredEdgeIDs{Application::instance().canvas().getHoveredEdgeIDs()};
    auto &selectedEdgeIDs{Application::instance().canvas().getSelectedEdgeIDs()};

    float vertexRadius{Application::getValue<Setting, float>(Setting::GRAPH_VERTEX_RADIUS)};
    float edgeThickness{Application::getValue<Setting, float>(Setting::GRAPH_EDGE_THICKNESS)};

    for(const auto &edge : edges_){
        if(isVertexHidden(edge->startID())
        || isVertexHidden(edge->endID())
        ){
            continue;
        }

        const auto &weightInputLabel{Application::instance().canvas().getWeightInputLabel()};

        bool isSelected{false};

        float thisThickness{edgeThickness};
        if(hoveredEdgeIDs == std::pair{edge->startID(), edge->endID()}){
            thisThickness *= 1.25f;
        }
        if(selectedEdgeIDs.find(
            std::pair{edge->startID(), edge->endID()}
            ) != selectedEdgeIDs.end()
        ){
            thisThickness *= 1.5f;
            isSelected = true;
        }

        std::stringstream weightLabelStream;
        weightLabelStream << std::fixed << std::setprecision(Application::getValue<Setting, int>(Setting::GRAPH_WEIGHT_PRECISION)) << edge->weight();
    
        const std::string &weightLabel{(!weightInputLabel.empty() && isSelected ? weightInputLabel.c_str() : weightLabelStream.str())};
    
        Vector2 lineMiddlePoint{Vector2Lerp(vertices_[edge->startID()]->position(), vertices_[edge->endID()]->position(), .5f)};


        float textHeight{thisThickness * vertexRadius / edgeThickness};
        float textWidth{static_cast<float>(MeasureText(weightLabel.c_str(), textHeight))};
    
        float padding{20.0f};
    
        Rectangle labelRectangle{
            lineMiddlePoint.x - (textWidth + padding) / 2.0f,
            lineMiddlePoint.y - textHeight / 2.0f,
            textWidth,
            textHeight
        };
    
        Rectangle textBackgroundRectangle{
            labelRectangle.x,
            labelRectangle.y,
            textWidth + padding,
            textHeight
        };
        
        Color edgeColor{edge->color()};
        Color backgroundColor{Fade(edgeColor, .25f)};
        float backgroundBrightness{calculateBrightness(edgeColor)};
        Color textColor{(backgroundBrightness > 128.0f) ? ColorBrightness(edgeColor, -.5f) : ColorBrightness(edgeColor, .5f)};
    
        DrawRectangleRounded(textBackgroundRectangle, .7f, 1.0f, backgroundColor);
    
        DrawText(
            weightLabel.c_str(),
            labelRectangle.x + padding / 2.0f,
            labelRectangle.y,
            textHeight,
            textColor
        );
    }
}

float Graph::calculateBrightness(Color color) const{
    // https://stackoverflow.com/questions/596216/formula-to-determine-perceived-brightness-of-rgb-color
    return .299f * color.r + .587f * color.g + .114f * color.b;
}