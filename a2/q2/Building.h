#ifndef BUILDING_H_
#define BUILDING_H_

#include "BCode.h"

class Building {
public:
    Building(const BCode&, const std::string&);
    std::string name() const;                            // accessor for name
    BCode bcode() const;                            // accessor for bcode

private:
    BCode bcode_;                                   // stores the building code
    std::string name_;                                   // stores the official name of the building
};

std::ostream& operator<< (std::ostream& os, const Building& a);

#endif //BUILDING_H_
