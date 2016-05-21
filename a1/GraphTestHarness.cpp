#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;
//************************************************************************
//* YOUR IMPLENTATION OF BCODE, BUILDING, COLLECTION HERE
//************************************************************************
class BCode {
public:
    BCode(const string&);                                       // constructor
    string code() const;

private:
    string code_;
    static int const min_length_ = 1;
    static int const max_length_ = 3;
};

BCode::BCode(const string& bcode) {
    // do a range check on the string
    if (bcode.length() < min_length_ || bcode.length() > max_length_)
        exit(1);
    // check if the starting character is a capital letter
    if (bcode[0] < 'A' || bcode[0] > 'Z')
        exit(1);

    code_ = bcode;
}

string BCode::code() const {
    return code_;
}

// comparison operators
bool operator== (const BCode& a, const BCode& b) {
    return a.code().compare(b.code()) == 0;
}

bool operator< (const BCode& a, const BCode& b) {
    for (int i = 0; i < a.code().length() && i < b.code().length(); i++) {
        // compare character by character, assumes all characters are upper case for now
        if (a.code()[i] < b.code()[i]) {
            return true;
        }
        else if (a.code()[i] > b.code()[i]) {
            return false;
        }
    }
    // if one of the codes run out of characters, the bigger string is considered greater
    return a.code().length() < b.code().length();
}

bool operator!= (const BCode& a, const BCode& b) {
    return !(a == b);
}

bool operator> (const BCode& a, const BCode& b) {
    return (a != b) && (b < a);
}

bool operator>= (const BCode& a, const BCode& b) {
    return (a > b) || (a == b);
}

bool operator<= (const BCode& a, const BCode& b) {
    return (a < b) || (a == b);
}

// operator overloads
ostream& operator<< (ostream& os, const BCode& a) {
    os << a.code(); return os; } 
// Building
class Building {
public:
    Building(const BCode&, const string&);
    string name() const;
    BCode bcode() const;

private:
    BCode bcode_;
    string name_;
};

// constructor
Building::Building(const BCode& bcode, const string& name)
    : bcode_(bcode), name_(name) {}

string Building::name() const {
    return name_;
}

BCode Building::bcode() const {
    return bcode_;
}

// operator overloads
ostream& operator<< (ostream& os, const Building& a) {
    os << a.bcode() << "\t" << a.name();
    return os;
}


//Collection
class Collection {
public:
    Collection();
    ~Collection();
    void insert(const BCode&, const string&);
    void insert(Building*);
    void remove(const BCode&);
    Building* findBuilding(const BCode&) const;
private:
    struct Node {
        Building* value;
        Node* next;
    };
    Node* buildings_;
};

// constructor
Collection::Collection() {
    buildings_ = NULL;
}

// destructor
Collection::~Collection() {
    Node* temp;
    while (buildings_) {
        temp = buildings_->next;
        delete buildings_->value;
        buildings_->value = NULL;
        delete buildings_;
        buildings_ = temp;
        temp = NULL;
    }
    delete temp;
}

void Collection::insert(const BCode& bcode, const string& name) {
    insert(new Building(bcode, name));
}

void Collection::insert(Building* building) {
    Node* temp = new Node();
    temp->value = building;
    temp->next = buildings_;
    buildings_ = temp;
    temp = NULL;
}


void Collection::remove(const BCode& bcode) {
    // indirect points to the address of the node we want to remove
    Node** indirect = &buildings_;
    
    while ((*indirect)->value->bcode() != bcode || (*indirect) != NULL)
        indirect = &(*indirect)->next;
    
    // replace it with whatever the next pointer is
    if (*indirect != NULL) {
        delete (*indirect)->value;
        *indirect = (*indirect)->next;
    }
}

Building* Collection::findBuilding(const BCode& bcode) const {
    // Go through the collection
    Node* temp = buildings_;
    
    while (temp) {
        if (temp->value->bcode() == bcode) {
            return temp->value;
        }
        temp = temp->next;
    }
    temp = NULL;
    delete temp;
    // return null pointer if building can't be found
    return NULL;
}



//===================================================================
// Graph (of Buildings and Connectors)
//===================================================================

class Graph {
public:
    Graph();                                                // constructor
    ~Graph();                                               // destructor
    Graph ( const Graph& );                                 // copy constructor
    void addNode ( Building* );                             // mutator - add node to graph
    void removeNode ( string );                             // mutator - remove node from graph
    Building* findBuilding ( string ) const;                // accessor - find building stored in node in graph
    void addEdge ( string, string, string );                // mutator - add edge to graph
    void removeEdge ( string, string );                     // mutator - remove edge from graph
    void printPaths ( string, string, const bool = false ) const; // accessor - print path from one node to another
    void deleteGraph();                                     // delete graph
    friend ostream& operator<< ( ostream&, const Graph& );  // insertion operator (insert graph into output stream)
    Graph& operator= ( const Graph& );                      // assignment operator for graph objects
    bool operator== ( const Graph& ) const;                 // equality operator for graph objects
private:
    struct Edge;    //forward declare edge
    struct Node {
        Building* value;  // source building
        Edge* paths;    // list of paths going out the building
        Node* next;
    };

    struct Edge {
        string type;    // the type of edge (e.g. bridge, tunnel, etc.)
        Node* to;     // destination building
        Edge* next;
    };
    
    Node* nodes_;
    Node* findNode(const string&) const;
    void addEdge(Node*, Node*, const string&);
    void removeEdge(Node*, const string&);
    void removeNode(Node*);
    bool findPath(Collection*, Node*, const string&) const;
};

//************************************************************************
//* YOUR IMPLENTATION OF GRAPH HERE
//************************************************************************
// constructor
Graph::Graph() {
    nodes_ = NULL;
}

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
        edge_walk = NULL;
        p_edge_walk = NULL;
        walk = &(*walk)->next;
        p_walk = p_walk->next;
    }
    walk = NULL;
    p_walk = NULL;

}

void Graph::addNode(Building* building) {
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
    // if its the first element
    if (prev == NULL) {
        new_node->next = temp;
        nodes_ = new_node;
    }
    else {
        prev->next = new_node;
        new_node->next = temp;
    }
    prev = NULL;
    temp = NULL;
}

void Graph::removeNode(string bcode) {
    Node* node_to_remove = findNode(bcode);
    removeNode(node_to_remove);
}

Building* Graph::findBuilding(string bcode) const {
    Node* node = findNode(bcode);
    if (node != NULL) {
        return node->value;
    }
    return NULL;
}
        
void Graph::addEdge(string code1, string code2, string connector) {
    // store the two nodes corresponding to the bcodes
    Node* node1 = findNode(code1);
    Node* node2 = findNode(code2);

    // first add edge from 1 to 2
    addEdge(node1, node2, connector);

    // now add edge from 2 to 1
    addEdge(node2, node1, connector);
}

void Graph::removeEdge(string code1, string code2) {
    Node* node1 = findNode(code1);
    Node* node2 = findNode(code2);

    // first remove edge from 1 to 2
    removeEdge(node1, code2);

    // now remove edge from 2 to 1
    removeEdge(node2, code1);
}

void Graph::printPaths(string code1, string code2, const bool one_line) const {
    Node* dest = findNode(code2);
    
    Collection* discovered_nodes = new Collection();
    discovered_nodes->insert(dest->value);

    //walk over the different edges 
    Edge* walk = dest->paths;
    while (walk != NULL) {
        //edge case if the path is immediate
        if (code1.compare(walk->to->value->bcode().code()) == 0) {
            cout << "\t" << walk->to->value->bcode()<< " (" << walk->type << ") " << code2 << endl;
        }
            
        else if (findPath(discovered_nodes, walk->to, code1)) {
            //print the first edge
            cout << walk->to->value->bcode()<< " (" << walk->type << ") " << code2 << endl;
        }
        walk = walk->next;
    }
}

void Graph::deleteGraph() {
    while (nodes_ != NULL) {
        removeNode(nodes_);
    }
}    

Graph::Node* Graph::findNode(const string& code) const {
    Node* temp = nodes_;
    BCode bcode(code);
    while (temp != NULL) {
        if (temp->value->bcode() == bcode) {
            return temp;
        }
        temp = temp->next;
    }
    delete temp;
    // return null pointer if building can't be found
    return NULL;
}
    
void Graph::addEdge(Node* src, Node* dest, const string& connector) {
    Edge* new_edge = new Edge();
    new_edge->type = connector;
    new_edge->to = dest;
    new_edge->next = src->paths;
    src->paths = new_edge;
    new_edge = NULL;
}

void Graph::removeEdge(Node* src, const string& bcode) {
    Edge** indirect = &(src->paths);

    while ((*indirect)->to->value->bcode() != bcode || *indirect != NULL)
        indirect = &(*indirect)->next;

    if (*indirect != NULL) {
        (*indirect)->to = NULL; //clear the reference to the building pointer
        *indirect = (*indirect)->next;
    }
}

void Graph::removeNode(Node* node) {
    Node** indirect = &nodes_;

    while (*indirect != node || *indirect != NULL)
        indirect = &(*indirect)->next;
    
    // remove node members
    if (*indirect != NULL) {
        while ((*indirect)->paths != NULL) {
            //get rid of all the nodes attached to this node
            removeEdge((*indirect)->paths->to->value->bcode().code(), (*indirect)->value->bcode().code());
        }
        (*indirect)->value;
        *indirect = node->next;
    }
}

bool Graph::findPath(Collection* discovered, Node* start, const string& dest) const {
    // label node as discovered
    discovered->insert(start->value);
    
    // traverse all the paths
    Edge* walk = start->paths;
    while (walk != NULL) {
        BCode code = walk->to->value->bcode();
        // if we find the destination
        if (dest.compare(code.code()) == 0) {
            cout << "\t" << dest << " (" << walk->type << ") ";
            walk = NULL;
            return true;
        }

        // if the building is not discovered yet
        if (discovered->findBuilding(code) == NULL) {
            if(findPath(discovered, walk->to, dest)) {
                cout << walk->to->value->bcode() << " (" << walk->type << ") ";
                walk = NULL;
                return true;
            }
        }

        walk = walk->next;
    }
    walk = NULL;
    return false;
}

// Graph operators
ostream& operator<< (ostream& os, const Graph& a) {
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
        os << endl << endl;
        walk = walk->next;
    }
    return os;
}

Graph& Graph::operator= (const Graph& a) {
    deleteGraph();
    Graph* n = new Graph(a);
    return *n;
}

bool Graph::operator== (const Graph& g) const {
    ostringstream other;
    other << g;
    ostringstream this_os;
    this_os << *this;
    return other.str().compare(this_os.str()) == 0;
}


//************************************************************************
//  Test Harness Helper functions
//************************************************************************

//  test-harness operators
enum Op { NONE, mapPtr, building, wreckage, findB, node, remNode, edge, remEdge, delGraph, copyGraph, assignGraph, eq, path, print };

Op convertOp( string opStr ) {
    switch( opStr[0] ) {
        case 'm': return mapPtr;
        case 'b': return building;
        case 'w': return wreckage;
        case 'f': return findB;
        case 'n': return node;
        case 'v': return remNode;
        case 'e': return edge;
        case 'r': return remEdge;
        case 'd': return delGraph;
        case 'c': return copyGraph;
        case 'a': return assignGraph;
        case 'q': return eq;
        case 'p': return path;
        case 'g': return print;
        default: {
            return NONE;
        }
    }
}

//******************************************************************
// Test Harness for Graph ADT
//******************************************************************

int main( int argc, char *argv[] ) {
    Collection buildings;
    Graph map1, map2;

    // initialize buildings and map1 with input file, if present
    if ( argc > 1 ) {

        ifstream source(argv[1]);
        if ( source.fail() ) {
            cerr << "Error: Could not open file \"" << argv[1] << "\"." << endl;
            return 1;
        }

        // create a collection of buildings, and a map of buildings and interior links

        string type;
        source >> type;
        Op op = convertOp ( type );
        while ( !source.eof() ) {
            switch (op) {

                    // add a new building to the collection of Buildings, and add the building to map1
                case building : {
                    string code;
                    string name;
                    string name2;
                    source >> code >> name;
                    getline( source, name2 );
                    buildings.insert( code, name+name2 );
                    map1.addNode( buildings.findBuilding ( code ) );
                    break;
                }

                    // add a new link between two existing nodes in map1
                case edge: {
                    string code1, code2, type;
                    source >> code1 >> code2 >> type;
                    map1.addEdge( code1, code2, type );
                    string junk;
                    getline ( source, junk );
                    break;
                }

                default: { }
            }
            source >> type;
            op = convertOp( type );
        }
    }

    cout << map1;

    Graph* map = &map1;  // input commands affect which ever graph that map points to (map1 or map2)

    cout << "Test harness for Graph ADT:" << endl << endl;

    // get input command
    cout << "Command: ";
    string command;
    cin >> command;

    Op op = convertOp( command );

    while ( !cin.eof() ) {
        switch (op) {

                // set variable map to point to new graph (map1 or map2)
            case mapPtr: {
                string mapNo;
                cin >> mapNo;
                map = ( mapNo[0] == '1' ) ? &map1 : &map2;
                break;
            }

                // print the current map to the console
            case print: {
                cout << *map;
                break;
            }

                // add a new building to the collection of buildings
            case building : {
                string code;
                string name;
                string name2;
                cin >> code >> name;
                getline( cin, name2 );
                buildings.insert( code, name+name2 );
                break;
            }

                // add an existing building to the current map
            case node: {
                string code;
                cin >> code;
                map->addNode( buildings.findBuilding( code ) );
                string junk;
                getline( cin, junk );
                break;
            }

                // find a building in the current map
            case findB: {
                string code;
                cin >> code;
                Building *b = map->findBuilding ( code );
                if ( b ) {
                    cout << *b << endl;
                }
                else {
                    cout << "Couldn't find building " << code << endl;
                }
                cout << endl;
                string junk;
                getline( cin, junk );
                break;
            }

                // add a new link between existing graph nodes in the current map
            case edge: {
                string code1, code2, type;
                cin >> code1 >> code2 >> type;
                map->addEdge( code1, code2, type );
                string junk;
                getline ( cin, junk );
                break;
            }


                // delete the entire graph (no memory leak).  There is no change to the collection of Buildings.
            case delGraph: {
                map->deleteGraph();
                break;
            }

                // remove an existing edge from the current map
            case remEdge: {
                string code1, code2;
                cin >> code1 >> code2;
                map->removeEdge( code1, code2 );
                string junk;
                getline ( cin, junk );
                break;
            }

                // remove an existing node from the current map.  There is no change to the collection of Buildings.
            case remNode: {
                string code;
                cin >> code;
                map->removeNode( code );
                string junk;
                getline( cin, junk );
                break;
            }

                // remove an existing building from the collection of buildings.  The building also needs to be removed from the two maps, as well as all links involving the building
            case wreckage: {
                string code;
                cin >> code;
                map1.removeNode( code );
                map2.removeNode( code );
                buildings.remove ( code );
                string junk;
                getline ( cin, junk );
                break;
            }

                // check whether map1 is equal to map2
            case eq: {
                if ( map1 == map2 ) {
                    cout << "Maps 1 and 2 are equal." << endl;
                }
                else {
                    cout << "Maps 1 and 2 are NOT equal." << endl;
                }
                break;
            }

                // graph copy constructor
            case copyGraph: {
                Graph map3( *map );
                cout << map3;
                string junk;
                getline( cin, junk );
                break;
            }

                // graph assignment operator
            case assignGraph: {
                map1 = map2;
                cout << map1;
                break;
            }

                // find path(s) in graph from one building to another building
            case path: {
                string code1, code2, all;
                cin >> code1 >> code2 >> all;
                cout << "Paths from " << code1 << " to " << code2 << " are: " << endl;
                bool printall = ( all.length() > 0 && all.at(0) == 't' ) ? true : false;
                map->printPaths( code1, code2, printall );
                string junk;
                getline( cin, junk );
                break;
            }

            default: {
                cerr << "Invalid command." << endl;
            }
        }

        cout << "Command: ";
        cin >> command;
        op = convertOp( command );

    } // while cin OK

}

