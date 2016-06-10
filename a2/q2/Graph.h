#ifndef GRAPH_H_
#define GRAPH_H_

#include "Building.h"
#include "BaseException.h"
#include <array>

// Similar to collection, but stores linked list of only strings
class CodeList {
public:
    CodeList();
    ~CodeList();
    void insert(const std::string&);
    bool findCode(const std::string&) const;
private:
    struct Node {
        std::string value;
        Node* next;
    };
    Node* code_list_;
};

//===================================================================
// Graph (of Buildings and Connectors)
//===================================================================

class Graph {
public:
    // Exception classes for errors handled through Graph
    class NodeAlreadyExistsException { 
    public:
        NodeAlreadyExistsException () {}                    // Exception for existing node addition
    };
    class NodeNotFoundException {
    public:
        NodeNotFoundException (const std::string& code) : code_(code) {} //Exception for node not found
        std::string code() const { return code_; }
    private:
        std::string code_;
    };
    class NodeSelfConnectionException {
    public:
        NodeSelfConnectionException (const std::string& code)   // Exception for self connecting nodes
            : code_(code) {}
        std::string code() const { return code_; }
    private:
        std::string code_;
    };
    class EdgeException {
    public:
        EdgeException (const std::string& node1, const std::string& node2)  //Base exception class for edge related
            : node1_(node1), node2_(node2) {}                               //exceptions
        std::string node1() const { return node1_; }                        // accessor
        std::string node2() const { return node2_; }                        // accessor
    private:
        std::string node1_;
        std::string node2_;
    };
    class NoEdgeFoundException : public EdgeException {
    public:
        NoEdgeFoundException (const std::string& node1, const std::string& node2)//exception for edge not found
            : EdgeException(node1, node2) {}
    };
    class EdgeAlreadyExistsException : public EdgeException {
    public:
        EdgeAlreadyExistsException (const std::string& node1, const std::string& node2)//exception for edge already exists
            : EdgeException(node1, node2) {}
    };
    class ConnectorTypeException : public BaseException {
    public:
        ConnectorTypeException (const std::string& connector);               // exception for invalid connector type
    };


    Graph();                                                // constructor
    ~Graph();                                               // destructor
    Graph ( const Graph& );                                 // copy constructor
    void addNode ( Building* );                             // mutator - add node to graph
    void removeNode ( std::string );                        // mutator - remove node from graph
    Building* findBuilding ( std::string ) const;           // accessor - find building stored in node in graph
    void addEdge ( std::string, std::string, std::string ); // mutator - add edge to graph
    void removeEdge ( std::string, std::string );           // mutator - remove edge from graph
    void printPaths ( std::string, std::string, const bool = false ) const; // accessor - print path from one node to another
    void deleteGraph();                                     // delete graph
    friend std::ostream& operator<< ( std::ostream&, const Graph& );  // insertion operator (insert graph into output stream)
    Graph& operator= ( const Graph& );                      // assignment operator for graph objects
    bool operator== ( const Graph& ) const;                 // equality operator for graph objects
private:
    struct Edge;        //forward declare edge
    struct Node {
        Building* value;// source building
        Edge* paths;    // list of paths going out the building
        Node* next;
    };

    struct Edge {
        std::string type;    // the type of edge (e.g. bridge, tunnel, etc.)
        Node* to;       // destination building
        Edge* next;
    };
    
    Node* nodes_;                                               // holds the root of nodes list
    static std::array<std::string, 3> connectors;
    // helper functions, function signatures and names are self documenting
    Node* findNode(const std::string&) const;                   // find node given the bcode
    void addEdge(Node*, Node*, const std::string&);             // add edge given the nodes and connector
    void removeEdge(Node*, const std::string&);                 // remove edge given the node and destination bcode
    void removeNode(Node*);                                     // remove node given the node pointer
    void removePaths(Node*);                                    // remove all the paths attached to a node
    bool findPath(CodeList*, Node*, const std::string&) const;   // recursive function for depth-first search
};

#endif // GRAPH_H_
