#include "graph.h"

Graph::Graph()
    : isDirected_(false)
    , mode_(Mode::EDIT)
    , highlightedNode_(nullptr)
    , highlightedEdge_(nullptr)
    , connectFrom_(nullptr)
    , draggedNode_(nullptr)
{}

Graph::Graph(const std::vector<Node*> &nodes, const std::vector<Edge*> &edges)
    : Graph()
{
    nodes_ = nodes;
    edges_ = edges;
}

Graph::~Graph(){
    reset();
}

void Graph::draw() const{
    for(const auto &edge : edges_){ 
        edge->draw(edge == highlightedEdge_);
    }
    for(const auto &node : nodes_){ 
        node->draw(node == highlightedNode_);
    }
}

void Graph::update(){
    updateHighlightedNodeAndEdge();

    switch(mode_){
    case Mode::ADD:      updateAddMode(); break;
    case Mode::DRAG:     updateDragMode(); break;
    case Mode::CONNECT:  updateConnectMode(); break;
    case Mode::DEL_NODE: updateDeleteNodeMode(); break;
    case Mode::DEL_EDGE: updateDeleteEdgeMode(); break;
    case Mode::EDIT: default: break;
    }
}

void Graph::switchMode(Mode mode){ 
    highlightedEdge_ = nullptr;
    highlightedNode_ = nullptr;
    mode_ = mode;
}

void Graph::reset(){
    for(auto &node : nodes_){ delete node;}
    for(auto &edge : edges_){ delete edge;}
    nodes_.clear();
    edges_.clear();
}

Node *Graph::findNode(Vector2 position){
    for(size_t i{nodes_.size()}; i-- > 0;){
        if(nodes_[i]->contains(position)) return nodes_[i];
    }
    return nullptr;
}

Edge *Graph::findEdge(Vector2 position){
    for(size_t i{edges_.size()}; i-- > 0;){
        if(edges_[i]->contains(position)) return edges_[i];
    }
    return nullptr;
}

void Graph::updateHighlightedNodeAndEdge(){
    highlightedNode_ = findNode(GetMousePosition());
    highlightedEdge_ = findEdge(GetMousePosition());
}

void Graph::updateAddMode(){
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        if(CheckCollisionPointRec(GetMousePosition(), {5, 5, 82, 31 + 7 * 25})) return;
        addNode(GetMousePosition());
    }
}

void Graph::updateDragMode(){
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        draggedNode_ = findNode(GetMousePosition());
    }

    if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
        draggedNode_ = nullptr;
    }

    if(draggedNode_){
        draggedNode_->updatePosition(GetMousePosition());
    }
}

void Graph::updateConnectMode(){
    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
        Node *node{highlightedNode_};
        if(node){
            if(!connectFrom_){
                connectFrom_ = node;
            }else{
                if(addEdge(connectFrom_, node)){
                    connectFrom_ = node;
                }
            }
        }
    }

    if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) || IsKeyPressed(KEY_ESCAPE)){
        connectFrom_ = nullptr;
    }

    if(connectFrom_){
        DrawLineEx(connectFrom_->position(), GetMousePosition(), getEdgeThickness() * 1.25f, getEdgeColor());
        DrawCircleV(GetMousePosition(), getEdgeThickness() * .625f, getEdgeColor());
    }
}

void Graph::updateDeleteNodeMode(){
    if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
        if(!highlightedNode_) return;
        removeNode(highlightedNode_);
    }
}

void Graph::updateDeleteEdgeMode(){
    if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
        if(!highlightedEdge_) return;
        removeEdge(highlightedEdge_);
    }
}

void Graph::addNode(Vector2 position){
    auto node{new Node(position)};
    nodes_.emplace_back(node);
    std::cout << "Added node [ID: " << node->id() << "] at (" << position.x << ", " << position.y << ")" << std::endl;
}

void Graph::removeNode(Node *node){
    for(size_t i{edges_.size()}; i-- > 0;){
        if(edges_[i]->contains(*node)){
            removeEdge(edges_[i]);
        }
    }

    std::cout << "Removed node [ID: " << node->id() << "]" << std::endl;
    nodes_.erase(std::remove(nodes_.begin(), nodes_.end(), node), nodes_.end());
}

bool Graph::addEdge(Node *n1, Node *n2){
    if(n1 == n2) return false;
    for(const auto &edge : edges_){
        if(edge->isEqual(n1, n2, isDirected_)) return false;
    }

    auto edge{new Edge(n1, n2)};
    edges_.emplace_back(edge);

    std::cout << "Added edge [ID: " << edge->id() << "] from Node[" << n1->id() << "] to Node[" << n2->id() << "]" << std::endl;
    return true;
}

void Graph::removeEdge(Edge *edge){
    std::cout << "Removed edge [ID: " << edge->id() << "]" << std::endl;
    edges_.erase(std::remove(edges_.begin(), edges_.end(), edge), edges_.end());
}