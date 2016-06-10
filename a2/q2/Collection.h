#include <vector>
#include <algorithm>
#include "Building.h"

//Collection
class Collection {
public:
    // Exception classes for various errors handled through Collection
    class BuildingNotFoundException : public BaseException {
    public:
        BuildingNotFoundException (const std::string&);     // Exception for no building found
    };
    class BuildingAlreadyInUseException : public BaseException {
    public:
        BuildingAlreadyInUseException (const std::string&); // Exception for already used building
    };
    class DestroyedBuildingException : public BaseException {
    public:
        DestroyedBuildingException (const std::string&);    // Exception for former buildings
    };

    Collection();                                   // constructor
    ~Collection();                                  // destructor
    void insert(const BCode&, const std::string&);  // insert a building to the collection
    void remove(const BCode&);                      // removes a building from the collection
    Building* findBuilding(const BCode&) const;     // finds the building in the collection
private:
    // Node represents a singly linked list node
    struct Node {           
        Building* value;
        Node* next;
    };
    Node* buildings_;                                   // stores the linked list node
    static std::vector<std::string> wrecked_buildings;  // stores the list of destroyed buildings
    static bool isWrecked (const BCode& bcode);         // checks if the given BCode has been destroyed
};
