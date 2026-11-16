// CardDeck implementation will be added here.

#include "../include/CardDeck.h"
#include "../include/Types.h"

CardDeck::CardDeck(){
    FaceAnimal animals[] = {
        Crab, Penguin, Octopus, Turtle, Walrus
    };

    FaceBackground backgrounds[] = {
        Red, Green, Purple, Blue, Yellow
    };

    for (FaceAnimal animal : animals) {
        for (FaceBackground background : backgrounds) {
            deck.push_back(new Card(animal, background));
        }
    }
}

static CardDeck& CardDeck::make_CardDeck(){
    static CardDeck instance; // now it will be same throughout the entire program
    return instance;
}