// Board implementation will be added here.

#include "../include/Board.h"


Board::Board() {
    CardDeck& deck = CardDeck::make_CardDeck();
    
    for (int r = 0; r < 5; ++r) {
        for (int c = 0; c < 5; ++c) {
            
            if (r == 2 && c == 2) {
                cards[r][c] = nullptr;
                faceUp[r][c] = false;
                continue;
            }
            
            Card* nextCard = deck.getNext();
            if (nextCard == nullptr) {
                throw NoMoreCards("Not enough cards in the deck to construct the board.");
            }

            cards[r][c] = nextCard;
            faceUp[r][c] = false;
        }
    }
    
    // Initialize the string representation
    // updateDisplayBoard();
}


bool Board::isValid(const Letter& letter, const Number& number) const {
    int r = static_cast<int>(letter);
    int c = static_cast<int>(number);

    return (r >= 0 && r < 5 && c >= 0 && c < 5 && !(r == 2 && c == 2));
}

bool Board::isFaceUp(const Letter& letter, const Number& number) const {
    int row = static_cast<int>(letter);
    int col = static_cast<int>(number);

    if(!isValid(letter, number)){
        throw OutOfRange("Invalid card position or center position C3.");
    }

    return faceUp[row][col];
}

bool Board::turnFaceUp(const Letter& letter, const Number& number){
    int row = static_cast<int>(letter);
    int col = static_cast<int>(number);

    if(!isValid(letter, number)){
        throw OutOfRange("Invalid card position or center position C3.");
    }

    if (faceUp[row][col]){
        return false;
    }

    faceUp[row][col] = true;
    return true; 
}

bool Board::turnFaceDown(const Letter& letter, const Number& number){
    int row = static_cast<int>(letter);
    int col = static_cast<int>(number);

    if(!isValid(letter, number)){
        throw OutOfRange("Invalid card position or center position C3.");
    }

    if (!faceUp[row][col]){
        return false;
    }

    faceUp[row][col] = false;
    return true; 
}

Card* Board::getCard(const Letter& letter, const Number& number){
    int row = static_cast<int>(letter);
    int col = static_cast<int>(number);

    if(!isValid(letter, number)){
        throw OutOfRange("Invalid card position or center position C3.");
    }

    return cards[row][col];
}

// is this correct?ho
void Board::setCard(const Letter& letter, const Number& number, Card* card){
    int row = static_cast<int>(letter);
    int col = static_cast<int>(number);

    if(!isValid(letter, number)){
        throw OutOfRange("Invalid card position or center position C3.");
    }
    cards[row][col] = card;
}

void Board::allFacesDown(){
    int ROWS = cards.size();
    int COLS = cards[0].size();

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            faceUp[row][col] = false;
        }
    }
}

std::ostream& operator<<(std::ostream& os, const Board& board){
    int ROWS = board.cards.size();
    int COLS = board.cards[0].size();

    for (int row = 0; row < ROWS; row++) {
        for(int cardRow = 0; cardRow < 3; cardRow++){
            for (int col = 0; col < COLS; col++) {
                if(row == 2 && col == 2){
                    os << "   ";
                }else{
                    Card* card = board.cards[row][col];

                    if(board.faceUp[row][col]){
                        os << (*card)(cardRow);
                    }else{
                        os << "zzz";
                    }
                }

                if(col < 4){
                    os << " ";
                } 
            }
            os << "\n";
        }
    }

    return os;
}