#pragma once

// RubisDeck declarations will be added here.

#include "DeckFactory.h"
#include "Rubis.h"

class RubisDeck : public DeckFactory<Rubis>{
    public:
        static RubisDeck& make_RubisDeck();
    
    private:
        RubisDeck();
};