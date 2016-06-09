#include "Building.h"

//Collection
class Collection {
public:
    class BuildingDoesNotExist {
    public:
        BuildingDoesNotExist (const BCode& bcode);
        std::string message() const { return message_; }
    private:
        std::string message_;
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
    Node* buildings_;                               // stores the linked list node
};
