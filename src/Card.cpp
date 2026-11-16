// Card implementation will be added here.

#include "../include/Card.h"

Card::Card(FaceAnimal animal, FaceBackground background) : animal(animal), background(background){}

int Card::getNRows() const{
    return 3;
}

Card::operator FaceAnimal() const{
    return animal;
}

Card::operator FaceBackground() const{
    return background;
}

// Returns the string representation of the requested row
std::string Card::operator()(int row) const {
    if (row < 0 || row >= getNRows()) {
        std::cout << "Invalid card row" << std::endl;
        return "";
    }

    char animalChar;
    char backgroundChar;

    // Convert animal enum to its corresponding character
    switch (animal) {
        case Crab:     animalChar = 'C'; break;
        case Penguin:  animalChar = 'P'; break;
        case Octopus:  animalChar = 'O'; break;
        case Turtle:   animalChar = 'T'; break;
        case Walrus:   animalChar = 'W'; break;
    }

    // Convert background enum to its corresponding lowercase character
    switch (background) {
        case Red:     backgroundChar = 'r'; break;
        case Green:   backgroundChar = 'g'; break;
        case Purple:  backgroundChar = 'p'; break;
        case Blue:    backgroundChar = 'b'; break;
        case Yellow:  backgroundChar = 'y'; break;
    }

    // Construct the requested row
    if (row == 1) {
        return std::string(1, backgroundChar) + animalChar + backgroundChar;
    }

    return std::string(3, backgroundChar);
}
