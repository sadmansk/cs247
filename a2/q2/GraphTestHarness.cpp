#include <iostream>
#include <cstdlib>
#include <fstream>
#include "Graph.h"
#include "Collection.h"

using namespace std;

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
                    try {
                        source >> code >> name;
                        getline( source, name2 );
                        buildings.insert( code, name+name2 );
                        map1.addNode( buildings.findBuilding ( code ) );
                    }
                    catch ( BaseException &e ) {
                        cout << e.message() << endl;

                        cout << "This command results in no changes to the collection of buildings or to maps." << endl;
                    }

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
    bool success = false;

    // Error strings
    const string no_change_map = "This command results in no changes to map";
    const string no_change = "This command results in no changes to the collection of buildings or to maps.";

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
                success = false;
                cin >> code >> name;
                getline( cin, name2 );
                while (!success && !cin.eof()) {
                    try {
                        buildings.insert( code, name+name2 );
                        success = true;
                    }
                    catch (BCode::InvalidFormatException &e) {
                        cout << e.message() << e.reason() << endl;
                        cout << "Please enter a new building code: ";
                        cin >> code;
                    }
                    catch (BaseException &e) {
                        cout << e.message() << endl;
                        cout << "Please enter a new building code: ";
                        cin >> code;
                    }
                }
                break;
            }

                // add an existing building to the current map
            case node: {
                string code;
                try {
                    cin >> code;
                    string junk;
                    getline( cin, junk );
                    map->addNode( buildings.findBuilding( code ) );
                }
                catch (BaseException &e) {
                    cout << e.message() << endl;
                    // get the current map
                    char mapNo = '0';
                    if (map == &map1) mapNo = '1';
                    else if (map == &map2) mapNo = '2';
                    cout << no_change_map << mapNo << '.' << endl;
                }
                catch (Graph::NodeAlreadyExistsException &e) {
                    // get the current map
                    char mapNo = '0';
                    if (map == &map1) mapNo = '1';
                    else if (map == &map2) mapNo = '2';
                    cout << "\nERROR: There is already a node for building \"" << code << "\" in map" << mapNo << '.' << endl;
                    cout << no_change_map << mapNo << '.' << endl;
                }
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
                try {
                    string code1, code2, type;
                    cin >> code1 >> code2 >> type;
                    string junk;
                    getline ( cin, junk );
                    map->addEdge( code1, code2, type );
                }
                catch (Graph::NodeNotFoundException &e) {
                    // get the current map
                    char mapNo = '0';
                    if (map == &map1) mapNo = '1';
                    else if (map == &map2) mapNo = '2';
                    cout << "\nERROR: There is no building \"" << e.code() << "\" in map" << mapNo << " to use in the new edge." << endl;
                    cout << no_change_map << mapNo << '.' << endl;
                }
                catch (Graph::NodeSelfConnectionException &e) {
                    // get the current map
                    char mapNo = '0';
                    if (map == &map1) mapNo = '1';
                    else if (map == &map2) mapNo = '2';
                    cout << "\nERROR: Cannot connect node \"" << e.code() << "\" to itself." << endl;
                    cout << no_change_map << mapNo << '.' << endl;
                }
                break;
            }


                // delete the entire graph (no memory leak).  There is no change to the collection of Buildings.
            case delGraph: {
                map->deleteGraph();
                break;
            }

                // remove an existing edge from the current map
            case remEdge: {
                try {
                    string code1, code2;
                    cin >> code1 >> code2;
                    string junk;
                    getline ( cin, junk );
                    map->removeEdge( code1, code2 );
                }
                catch (Graph::NodeNotFoundException &e) {
                    // get the current map
                    char mapNo = '0';
                    if (map == &map1) mapNo = '1';
                    else if (map == &map2) mapNo = '2';
                    cout << "\nERROR: There is no building \"" << e.code() << "\" in map" << mapNo << " to be removed." << endl;
                    cout << no_change_map << mapNo << '.' << endl;
                }
                catch (Graph::NoEdgeFoundException &e) {
                    // get the current map
                    char mapNo = '0';
                    if (map == &map1) mapNo = '1';
                    else if (map == &map2) mapNo = '2';
                    cout << "\nERROR: There is no edge in map" << mapNo << " between buildings \"" << e.node1() << "\" and \"" << e.node1() << "\" to be removed." << endl;
                    cout << no_change_map << mapNo << '.' << endl;
                }
                break;
            }

                // remove an existing node from the current map.  There is no change to the collection of Buildings.
            case remNode: {
                try {
                    string code;
                    cin >> code;
                    string junk;
                    getline( cin, junk );
                    map->removeNode( code );
                }
                catch (Graph::NodeNotFoundException &e) {
                    // get the current map
                    char mapNo = '0';
                    if (map == &map1) mapNo = '1';
                    else if (map == &map2) mapNo = '2';
                    cout << "\nERROR: There is no building \"" << e.code() << "\" in map" << mapNo << " to be removed." << endl;
                    cout << no_change_map << mapNo << '.' << endl;
                }
                break;
            }

                // remove an existing building from the collection of buildings.  The building also needs to be removed from the two maps, as well as all links involving the building
            case wreckage: {
                string code;
                try {
                    cin >> code;
                    string junk;
                    getline ( cin, junk );
                    map1.removeNode( code );
                    map2.removeNode( code );
                    buildings.remove ( code );
                }
                catch (Graph::NodeNotFoundException &e) {
                    try {
                        buildings.remove (code);
                    }
                    catch (BaseException &f) {
                        cout << f.message() << endl;
                        cout << no_change << endl;
                    }
                }
                catch (BaseException &e) {
                    cout << e.message() << endl;
                    cout << no_change << endl;
                }
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
                try {
                    string code1, code2, all;
                    cin >> code1 >> code2 >> all;
                    cout << "Paths from " << code1 << " to " << code2 << " are: " << endl;
                    bool printall = ( all.length() > 0 && all.at(0) == 't' ) ? true : false;
                    string junk;
                    getline( cin, junk );
                    map->printPaths( code1, code2, printall );
                }
                catch (Graph::NodeNotFoundException &e) {
                    // get the current map
                    char mapNo = '0';
                    if (map == &map1) mapNo = '1';
                    else if (map == &map2) mapNo = '2';
                    cout << "\nERROR: There is no building \"" << e.code() << "\" in map" << mapNo << "." << endl;
                }
                break;
            }

            default: {
                cerr << "Invalid command." << endl;
            }
        }
        cout << endl;
        cout << "Command: ";
        cin >> command;
        op = convertOp( command );

    } // while cin OK

}

