// CardDeck declarations will be added here.

#include "DeckFactory.h"
#include "Card.h"

class CardDeck : public DeckFactory<Card>{
    public:
        static CardDeck& make_CardDeck();
    
    private:
        CardDeck();
};