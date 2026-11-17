// RubisDeck implementation will be added here.

#include "RubisDeck.h"

RubisDeck::RubisDeck() {
    for (int value = 1; value <= 4; value++) {
        deck.push_back(new Rubis(value));
    }
}

RubisDeck& RubisDeck::make_RubisDeck() {
    static RubisDeck instance;
    return instance;
}