#include <iostream>
#include <cstdlib>
#include <string>

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


//Collection
class Collection {
public:
    Collection();
    ~Collection();
    void insert(const BCode&, const string&);
    Building* findBuilding(const BCode&);
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
    Node* temp = buildings_;
    while (temp) {
    }
}

void Collection::insert(const BCode& bcode, const string& name) {
    Node* temp = buildings_;
    while (temp != NULL) {
        temp = temp->next;
    }
    temp = new Node();
    temp->value = new Building(bcode, name);
    temp->next = NULL;
}

Building* Collection::findBuilding(const BCode& bcode) {
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
    struct Node {
        struct Edge;    //forward declare edge
        Building* src;  // source building
        Edge* paths;    // list of paths going out the building
    };

    struct Edge {
        string type;    // the type of edge (e.g. bridge, tunnel, etc.)
        Node* from;      // source node
        Node* to;     // destination nodes
    };

    struct NodeList {
        Node value;
        NodeList* next;
    };
    
    NodeList* nodes_;
};

//************************************************************************
//* YOUR IMPLENTATION OF GRAPH HERE
//************************************************************************
// constructor
Graph::Graph() {
    nodes_ = NULL;
}

Graph::~Graph() {}

Graph::Graph(const Graph&) {

}

void Graph::addNode(Building* building) {
}
/*
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
*/
