// Card implementation will be added here.

#include "../include/Card.h"
#include <iostream>

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
        case FaceAnimal::Crab:     animalChar = 'C'; break;
        case FaceAnimal::Penguin:  animalChar = 'P'; break;
        case FaceAnimal::Octopus:  animalChar = 'O'; break;
        case FaceAnimal::Turtle:   animalChar = 'T'; break;
        case FaceAnimal::Walrus:   animalChar = 'W'; break;
    }

    // Convert background enum to its corresponding lowercase character
    switch (background) {
        case FaceBackground::Red:     backgroundChar = 'r'; break;
        case FaceBackground::Green:   backgroundChar = 'g'; break;
        case FaceBackground::Purple:  backgroundChar = 'p'; break;
        case FaceBackground::Blue:    backgroundChar = 'b'; break;
        case FaceBackground::Yellow:  backgroundChar = 'y'; break;
    }

    // Construct the requested row
    if (row == 1) {
        return std::string(1, backgroundChar) + animalChar + backgroundChar;
    }

    return std::string(3, backgroundChar);
}
