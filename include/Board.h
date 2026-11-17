// Board declarations will be added here.

#include "Types.h"
#include "Card.h"
#include "CardDeck.h"

#include <ostream>
#include <string>
#include <stdexcept> // Custom exceptions
#include <array>

class Board{
    private:
        // 5x5 grid storing pointers to the cards on the table (center C3 is nullptr)
        std::array<std::array<Card*, 5>, 5> cards;

        // 5x5 grid tracking whether each card is currently face up (true) or face down (false)
        std::array<std::array<bool, 5>, 5> faceUp;

        bool isValid(const Letter&, const Number&) const;
        
    public:
        Board();

        bool isFaceUp(const Letter&, const Number&) const;
        bool turnFaceUp(const Letter&, const Number&);
        bool turnFaceDown(const Letter&, const Number&);

        Card* getCard(const Letter&, const Number&);
        void setCard(const Letter&, const Number&, Card*) ;

        void allFacesDown();

        friend std::ostream& operator<<(std::ostream& os, const Board& board);
};

class OutOfRange : public std::runtime_error {
    public:
        OutOfRange(const std::string& msg)
            : std::runtime_error(msg) {}
};

class NoMoreCards : public std::runtime_error {
    public:
        NoMoreCards(const std::string& message = "No more cards available in the deck")
            : std::runtime_error(message) {}
};