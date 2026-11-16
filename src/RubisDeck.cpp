// RubisDeck implementation will be added here.

#include "RubisDeck.h"

RubisDeck::RubisDeck() {
    for (int value = 1; value <= 4; value++) {
        deck.push_back(new Rubis(value)); // is this good? cause it says that number of cards with X rubies must be exact??
    }
}

RubisDeck& RubisDeck::make_RubisDeck() {
    static RubisDeck instance;
    return instance;
}