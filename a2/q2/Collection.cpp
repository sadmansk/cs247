#include "Collection.h"

Collection::BuildingNotFoundException::BuildingNotFoundException(const std::string& code) : BaseException(code) {
    message_ = "\nERROR: There is no building with the code \"" + code + "\".";
}

Collection::BuildingAlreadyInUseException::BuildingAlreadyInUseException(const std::string& code) : BaseException(code) {
    message_ = "\nERROR: Building Code \"" + code + "\" is already in use.";
}

Collection::DestroyedBuildingException::DestroyedBuildingException(const std::string& code) : BaseException(code) {
    message_ = "\nERROR: Building Code \"" + code + "\" was used for a former building.";
}

std::vector<std::string> Collection::wrecked_buildings(0);
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
    if (findBuilding(bcode) != NULL) {
        throw BuildingAlreadyInUseException(bcode.code());
    }
    if (isWrecked(bcode)) {
        throw DestroyedBuildingException(bcode.code());
    }

    Building* building = new Building(bcode, name);
    Node* temp = new Node();
    temp->value = building;
    temp->next = buildings_;
    buildings_ = temp;
    temp = NULL;
    building = NULL;
}

void Collection::remove(const BCode& bcode) {
    // indirect points to the address of the node we want to remove
    Node** indirect = &buildings_;
    
    while ((*indirect) != NULL) {
        if ((*indirect)->value->bcode() != bcode)
            indirect = &(*indirect)->next;
        else 
            break;
    }
    
    // replace it with whatever the next pointer is
    if (*indirect != NULL) {
        //store the building in the list of wreckage
        wrecked_buildings.push_back((*indirect)->value->bcode().code());
        delete (*indirect)->value;
        Node* temp = *indirect;
        *indirect = temp->next;
        delete temp;
    }
    else {
        throw BuildingNotFoundException(bcode.code());
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

// checks if the given bcode is in the list of destroyed buildings
bool Collection::isWrecked (const BCode& bcode) {
    return std::find(std::begin(wrecked_buildings), std::end(wrecked_buildings), bcode.code()) != std::end(wrecked_buildings);
}
