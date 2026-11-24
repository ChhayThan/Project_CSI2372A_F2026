// CardDeck implementation will be added here.

#include "../include/CardDeck.h"
#include "../include/Types.h"

CardDeck::CardDeck(){
    FaceAnimal animals[] = {
        FaceAnimal::Crab, FaceAnimal::Penguin, FaceAnimal::Octopus, FaceAnimal::Turtle, FaceAnimal::Walrus
    };

    FaceBackground backgrounds[] = {
        FaceBackground::Red, FaceBackground::Green, FaceBackground::Purple, FaceBackground::Blue, FaceBackground::Yellow
    };

    for (FaceAnimal animal : animals) {
        for (FaceBackground background : backgrounds) {
            deck.push_back(new Card(animal, background));
        }
    }
    shuffle();
}

CardDeck& CardDeck::make_CardDeck(){
    static CardDeck instance; // now it will be same throughout the entire program
    return instance;
}