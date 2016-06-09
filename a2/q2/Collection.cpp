#include "Collection.h"

// constructor for safety
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
}

void Collection::insert(const BCode& bcode, const std::string& name) {
    Node* temp = new Node();
    Building* building = new Building(bcode, name);
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
