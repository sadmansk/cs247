#include "Graph.h"
#include <sstream>
#include <iostream>

// helper list to contain nodes
CodeList::CodeList() {
    code_list_ = NULL;
}

CodeList::~CodeList() {
    while (code_list_ != NULL) {
        Node* walk = code_list_->next;
        delete code_list_;
        code_list_ = walk;
    }
}

void CodeList::insert(const std::string& code) {
    Node* temp = new Node();
    temp->value = code;
    temp->next = code_list_;
    code_list_ = temp;
}
        
bool CodeList::findCode(const std::string& code) const {
    Node* temp = code_list_;

    while (temp) {
        if (temp->value.compare(code) == 0) {
            return true;
        }
        temp = temp->next;
    }
    return false;
}

// exception constructor
Graph::ConnectorTypeException::ConnectorTypeException (const std::string& connector)
   : BaseException(connector) {
    message_ = "\nERROR: There is no building connector of type \"" + connector + "\"."; 
}

// stores the valid connectors
std::array<std::string, 3> Graph::connectors = {"bridge", "tunnel", "hall"};

// constructor
Graph::Graph() {
    nodes_ = NULL;
}

// destructor
Graph::~Graph() {
    deleteGraph();
    delete nodes_;
}
    
Graph::Graph(const Graph& g) {
    // set the nodes list to null if the nodes_ in g are NULL
    if (g.nodes_ == NULL) {
        nodes_ = NULL;
        return;
    }
    Node** walk = &nodes_;
    Node* p_walk = g.nodes_;

    while (p_walk != NULL) {
        *walk = new Node();
        // building shouldn't be deep copied
        (*walk)->value = p_walk->value;

        // declare paths as null for now
        (*walk)->paths = NULL;
        (*walk)->next = NULL;
        walk = &(*walk)->next;
        p_walk = p_walk->next;
    }
    walk = &nodes_;
    p_walk = g.nodes_;
    //now go through the list of nodes and add their paths
    while (*walk != NULL && p_walk != NULL) {
        Edge* p_edge_walk = p_walk->paths;
        Edge** edge_walk = &((*walk)->paths);
        while (p_edge_walk != NULL) {
            *edge_walk = new Edge();
            (*edge_walk)->type = p_edge_walk->type;
            (*edge_walk)->to = findNode((p_edge_walk->to->value->bcode()).code());
            (*edge_walk)->next = NULL;
            edge_walk = &(*edge_walk)->next;
            p_edge_walk = p_edge_walk->next;
        }
        walk = &(*walk)->next;
        p_walk = p_walk->next;
    }

}

void Graph::addNode(Building* building) {
    if (findBuilding(building->bcode().code()) != NULL) {
        throw NodeAlreadyExistsException();
    }

    //declare a new node
    Node* new_node = new Node();
    new_node->value = building;
    new_node->paths = NULL;
    new_node->next = NULL;
    
    Node* temp = nodes_;
    Node* prev = NULL;
    while (temp != NULL) {
        if (building->bcode() < temp->value->bcode()) {
            break;
        }
        prev = temp;
        temp = temp->next;
    }
    // if its the first element set root to the new node
    if (prev == NULL) {
        new_node->next = temp;
        nodes_ = new_node;
    }
    else {
        prev->next = new_node;
        new_node->next = temp;
    }
}

void Graph::removeNode(std::string bcode) {
    Node* node_to_remove = findNode(bcode);
    if (node_to_remove == NULL) {
        throw NodeNotFoundException(bcode);
    }
    removeNode(node_to_remove);
}

Building* Graph::findBuilding(std::string bcode) const {
    Node* node = findNode(bcode);
    if (node != NULL) {
        return node->value;
    }
    return NULL;
}
        
void Graph::addEdge(std::string code1, std::string code2, std::string connector) {
    // store the two nodes corresponding to the bcodes
    Node* node1 = findNode(code1);
    Node* node2 = findNode(code2);

    // checks for exceptions
    if (node1 == NULL) {
        throw NodeNotFoundException(code1);
    }
    if (node2 == NULL) {
        throw NodeNotFoundException(code2);
    }
    if (node1 == node2) {
        throw NodeSelfConnectionException(code1);
    }

    // check if the edge already exists
    Edge* walk = node1->paths;

    while (walk != NULL) {
        if (walk->to->value->bcode().code().compare(code2) == 0) {
            break;
        }
        walk = walk->next;
    }
    // throw an exception if the path is not found
    if (walk != NULL) {
        throw EdgeAlreadyExistsException(code1, code2);
    }
    walk = NULL;

    //check if the connector type is valid
    bool isValidConnector = false;
    for (unsigned int i = 0; i < connectors.size(); ++i) {
        if (connectors[i] == connector) {
            isValidConnector = true;
            break;
        }
    }
    if (!isValidConnector) {
        throw ConnectorTypeException(connector);
    }

    // first add edge from 1 to 2
    addEdge(node1, node2, connector);

    // now add edge from 2 to 1
    addEdge(node2, node1, connector);
}

void Graph::removeEdge(std::string code1, std::string code2) {
    // store the two nodes corresponding to the bcodes
    Node* node1 = findNode(code1);
    Node* node2 = findNode(code2);

    // check for exceptions
    if (node1 == NULL) {
        throw NodeNotFoundException(code1);
    }
    if (node2 == NULL) {
        throw NodeNotFoundException(code2);
    }

    // first remove edge from 1 to 2
    removeEdge(node1, code2);

    // now remove edge from 2 to 1
    removeEdge(node2, code1);
}

void Graph::printPaths(std::string code1, std::string code2, const bool one_line) const {
    Node* dest = findNode(code2);
    
    // make sure that both the nodes exist
    if (findNode(code1) == NULL) {
        throw NodeNotFoundException(code1);
    }
    if (dest == NULL) {
        throw NodeNotFoundException(code2);
    }
    
    // hold the traversed nodes
    CodeList* discovered_nodes = new CodeList();
    discovered_nodes->insert(code2);

    //walk over the different edges 
    Edge* walk = dest->paths;
    while (walk != NULL) {
        //edge case if the path is immediate
        if (code1.compare(walk->to->value->bcode().code()) == 0) {
            std::cout << "\t" << walk->to->value->bcode()<< " (" << walk->type << ") " << code2 << std::endl;
            if (!one_line) break;
        }
        else if (findPath(discovered_nodes, walk->to, code1)) {
            //print the first edge
            std::cout << walk->to->value->bcode()<< " (" << walk->type << ") " << code2 << std::endl;
            if (!one_line) break;
        }
        walk = walk->next;
    }

    delete discovered_nodes;
}

void Graph::deleteGraph() {
    while (nodes_ != NULL) {
        removeNode(nodes_);
    }
}    

Graph::Node* Graph::findNode(const std::string& code) const {
    Node* temp = nodes_;
    BCode bcode(code);
    while (temp != NULL) {
        if (temp->value->bcode() == bcode) {
            return temp;
        }
        temp = temp->next;
    }
    // return null pointer if building can't be found
    return NULL;
}
    
void Graph::addEdge(Node* src, Node* dest, const std::string& connector) {
    Edge* new_edge = new Edge();
    new_edge->type = connector;
    new_edge->to = dest;
    new_edge->next = src->paths;
    src->paths = new_edge;
}

void Graph::removeEdge(Node* src, const std::string& bcode) {
    Edge* walk = src->paths;
    Edge* prev = NULL;

    while (walk != NULL) {
        if (walk->to->value->bcode().code().compare(bcode) == 0) {
            break;
        }
        prev = walk;
        walk = walk->next;
    }
    // throw an exception if the path is not found
    if (walk == NULL) {
        throw NoEdgeFoundException(src->value->bcode().code(), bcode);
    }
    walk->to = NULL; //clear the reference to the building pointer
    if (prev != NULL) {
        prev->next = walk->next;
    }
    else {
        src->paths = walk->next;
    }
    delete walk;
}

void Graph::removeNode(Node* node) {
    Node* walk = nodes_;
    Node* prev = NULL;

    while (walk != NULL && walk != node) {
        prev = walk;
        walk = walk->next;
    }
    removePaths(walk);
    walk->value = NULL;
    // remove node members
    if (prev != NULL) {
        prev->next = walk->next;
    }
    else {
        nodes_ = walk->next;
    }
    delete walk;
}

// remove all the paths attached to a node
void Graph::removePaths(Node* node) {
    while (node->paths != NULL) {
        removeEdge(node->value->bcode().code(), node->paths->to->value->bcode().code());
    }
}

bool Graph::findPath(CodeList* discovered, Node* start, const std::string& dest) const {
    // label node as discovered
    discovered->insert(start->value->bcode().code());
    
    // traverse all the paths
    Edge* walk = start->paths;
    while (walk != NULL) {
        BCode code = walk->to->value->bcode();
        // if we find the destination
        if (dest.compare(code.code()) == 0) {
            std::cout << "\t" << dest << " (" << walk->type << ") ";
            return true;
        }

        // if the building is not discovered yet
        if (!discovered->findCode(code.code())) {
            if(findPath(discovered, walk->to, dest)) {
                std::cout << walk->to->value->bcode() << " (" << walk->type << ") ";
                return true;
            }
        }
        walk = walk->next;
    }
    return false;
}

// Graph operators
std::ostream& operator<< (std::ostream& os, const Graph& a) {
    //traverse the nodes list
    Graph::Node* walk = a.nodes_;
    while (walk) {
        os << *(walk->value); // print the building info
        // print the paths
        Graph::Edge* walk_edge = walk->paths;
        if (walk_edge != NULL) {
            os << "\n\tConnects to: ";
        }
        while (walk_edge != NULL) {
            os << walk_edge->to->value->bcode() << " (" << walk_edge->type << ")";
            walk_edge = walk_edge->next;
            if (walk_edge != NULL) {
                os << ", ";
            }
        }
        os << std::endl << std::endl;
        walk = walk->next;
    }
    return os;
}

Graph& Graph::operator= (const Graph& a) {
    deleteGraph(); // make sure our current graph is empty before copying

    // Apply copy swap idiom
    Graph copy{a};
    Node* temp;

    temp = copy.nodes_;
    copy.nodes_ = nodes_;
    nodes_ = temp;

    return *this;
}

bool Graph::operator== (const Graph& g) const {
    // hold the std::ostream data for each of the graphs
    std::ostringstream other;
    other << g;
    std::ostringstream this_os;
    this_os << *this;
    // compare the std::ostream strings
    return other.str().compare(this_os.str()) == 0;
}
