#include "Building.h"

// constructor
Building::Building(const BCode& bcode, const std::string& name)
    : bcode_(bcode), name_(name) {}

std::string Building::name() const {
    return name_;
}

BCode Building::bcode() const {
    return bcode_;
}

// operator overloads
std::ostream& operator<< (std::ostream& os, const Building& a) {
    os << a.bcode() << "\t" << a.name();
    return os;
}
