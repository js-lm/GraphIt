#include "graph.h"
#include "system/application.h"
#include "canvas/canvas.h"

#include <raylib.h>
#include <raymath.h>
#include <iomanip>
#include <sstream>

void Graph::draw() const{
    drawEdges();
    drawVertices();
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

void Graph::drawEdges() const{
    auto &hoveredEdgeIDs{Application::instance().canvas().getHoveredEdgeIDs()};
    auto &selectedEdgeIDs{Application::instance().canvas().getSelectedEdgeIDs()};

    const auto &weightTempLabel{Application::instance().canvas().getWeightTempLabel()};
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

        bool isSelected{false};

        // highlight aura
        if(selectedEdgeIDs.find(
            std::pair{edge->startID(), edge->endID()}
            ) != selectedEdgeIDs.end()
        ){
            isSelected = true;

            DrawLineEx(
                startPoint, 
                endPoint, 
                edgeThicknessForThisEdge * 2.0f, 
                Fade(edge->color(), .5f)
            );

            drawArrowLine(
                startPoint, 
                endPoint, 
                edgeThicknessForThisEdge * 1.5f, 
                "",
                Fade(edge->color(), .5f)
            );
        }

        std::stringstream weightLabel;
        weightLabel << std::fixed << std::setprecision(Application::getValue<Setting, int>(Setting::GRAPH_WEIGHT_PRECISION)) << edge->weight();
        drawArrowLine(
            startPoint, 
            endPoint, 
            edgeThickness, 
            (!weightTempLabel.empty() && isSelected ? weightTempLabel.c_str() : weightLabel.str()), 
            edge->color()
        );
    }
}

void Graph::drawArrowLine(const Vector2 &startPoint, const Vector2 &endPoint, float thickness, const std::string &label, Color color) const{
    float vertexRadius{Application::getValue<Setting, float>(Setting::GRAPH_VERTEX_RADIUS)};
    float edgeThickness{Application::getValue<Setting, float>(Setting::GRAPH_EDGE_THICKNESS)};

    Vector2 arrowVector{Vector2Subtract(endPoint, startPoint)};

    Vector2 arrowDirection{Vector2Normalize(arrowVector)};
    float originalDistance{Vector2Distance(startPoint, endPoint)};

    Vector2 lineVector{arrowDirection * (originalDistance - vertexRadius)};
    Vector2 labelVector{arrowDirection * (originalDistance / 2)};

    Vector2 arrowEndPoint{Vector2Add(startPoint, lineVector)};
    Vector2 arrowMiddlePoint{Vector2Add(startPoint, labelVector)};

    // main line
    DrawLineEx(startPoint, arrowEndPoint, thickness, color);

    // arrow
    if(Application::getValue<Setting, bool>(Setting::GRAPH_IS_DIRECTED)){
        Vector2 direction{Vector2Normalize(Vector2Subtract(arrowEndPoint, startPoint))};
        Vector2 rotateAngel1{Vector2Rotate(direction, 70.0f * DEG2RAD)};
        Vector2 rotateAngel2{Vector2Rotate(direction, 110.0f * DEG2RAD)};
    
        Vector2 arrow1{Vector2Add(arrowEndPoint, Vector2Scale(Vector2Add(direction, rotateAngel1), thickness * -2.0f))};
        Vector2 arrow2{Vector2Add(arrowEndPoint, Vector2Scale(Vector2Subtract(direction, rotateAngel2), thickness * -2.0f))};
    
        DrawLineEx(arrowEndPoint, arrow1, thickness, color);
        DrawLineEx(arrowEndPoint, arrow2, thickness, color);
    }

    // label
    if(label.empty()
    || !Application::getValue<Setting, bool>(Setting::GRAPH_IS_LABELED)
    ){
        return;
    }

    float textHeight{thickness * vertexRadius / edgeThickness};
    float textWidth{static_cast<float>(MeasureText(label.c_str(), textHeight))};

    float padding{20.0f};

    Rectangle labelRectangle{
        arrowMiddlePoint.x - (textWidth + padding) / 2.0f,
        arrowMiddlePoint.y - textHeight / 2.0f,
        textWidth,
        textHeight
    };

    Rectangle textBackgroundRectangle{
        labelRectangle.x,
        labelRectangle.y,
        textWidth + padding,
        textHeight
    };

    Color backgroundColor{Fade(color, .25f)};
    float backgroundBrightness{calculateBrightness(color)};
    Color textColor{(backgroundBrightness > 128.0f) ? ColorBrightness(color, -.5f) : ColorBrightness(color, .5f)};

    DrawRectangleRounded(textBackgroundRectangle, .7f, 1.0f, backgroundColor);

    DrawText(
        label.c_str(),
        labelRectangle.x + padding / 2.0f,
        labelRectangle.y,
        textHeight,
        textColor
    );
}

float Graph::calculateBrightness(Color color) const{
    // https://stackoverflow.com/questions/596216/formula-to-determine-perceived-brightness-of-rgb-color
    return .299f * color.r + .587f * color.g + .114f * color.b;
}