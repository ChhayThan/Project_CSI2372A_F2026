// Rubis implementation will be added here.

#include "../include/Rubis.h"

Rubis::Rubis(int numberOfRubis) : rubies(numberOfRubis){}

Rubis::operator int() const{
    return rubies;
}

std::ostream& operator<<(std::ostream& os, const Rubis& rubie) {
    os << " Number of Rubies: " << static_cast<int>(rubie);
    return os;
}