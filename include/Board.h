// Board declarations will be added here.

#include "Types.h"
#include "Card.h"

#include <ostream>
#include <string>
#include <vector>

class Board{
    private:
        std::vector<std::string> displayBoard;

        // 5x5 grid storing pointers to the cards on the table (center C3 is nullptr)
        std::array<std::array<Card*, 5>, 5> cards;

        // 5x5 grid tracking whether each card is currently face up (true) or face down (false)
        std::array<std::array<bool, 5>, 5> faceUp;

        int numberOfCards;
        
    public:
        Board();

        bool isFaceUp(const Letter&, const Number&) const;
        bool turnFaceUp(const Letter&, const Number&);
        bool turnFaceDown(const Letter&, const Number&);

        Card* getCard(const Letter&, const Number&);
        void setCard(const Letter&, const Number&, Card*) ;

        void allFacesDown();

        friend std::ostream& operator<<(std::ostream& os, const Board& p);
};