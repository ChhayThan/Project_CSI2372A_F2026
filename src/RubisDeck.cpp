// RubisDeck implementation will be added here.

#include "../include/RubisDeck.h"

RubisDeck::RubisDeck() {
    int values[] = {1, 1, 1, 2, 2, 3, 4};
    for (int value : values) {
        deck.push_back(new Rubis(value));
    }
    shuffle();
}

RubisDeck& RubisDeck::make_RubisDeck() {
    static RubisDeck instance;
    return instance;
}