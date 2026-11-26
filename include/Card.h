#pragma once

// Card declarations will be added here.

#include "Types.h"
#include <string>

class Card{
    public:
        int getNRows() const;
        operator FaceAnimal() const;
        operator FaceBackground() const;
        std::string operator()(int) const;
    
    private:
        FaceAnimal animal;
        FaceBackground background;

        Card(FaceAnimal animal, FaceBackground background);
        friend class CardDeck;
};