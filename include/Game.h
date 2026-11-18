#pragma once

#include "Board.h"
#include "Player.h"

#include <iosfwd>
#include <vector>

// Encapsulates the board, players, round number, and selected cards.
class Game {
public:
    // Creates a game at round zero with no players or selected cards.
    Game();

    // Returns the current round number.
    int getRound() const;

    // Changes the round number to a value from zero through seven.
    void setRound(int round);

    // Adds a player, rejecting duplicate sides and games with over four players.
    void addPlayer(const Player& player);

    // Returns the player assigned to the requested side.
    Player& getPlayer(Side side);

    // Returns the player assigned to the requested side without allowing modification.
    const Player& getPlayer(Side side) const;

    // Returns all players in their insertion and turn order.
    const std::vector<Player>& getPlayers() const;

    // Returns the card selected immediately before the current card.
    const Card* getPreviousCard() const;

    // Returns the most recently selected card.
    const Card* getCurrentCard() const;

    // Selects a card and shifts the current card to previous; nullptr resets both.
    void setCurrentCard(const Card* card);

    // Returns whether the card at the requested position is face up.
    bool isFaceUp(const Letter& letter, const Number& number) const;

    // Turns the requested card face up and returns whether its state changed.
    bool turnFaceUp(const Letter& letter, const Number& number);

    // Turns the requested card face down and returns whether its state changed.
    bool turnFaceDown(const Letter& letter, const Number& number);

    // Turns every card on the board face down.
    void allFacesDown();

    // Returns the card pointer stored at the requested board position.
    Card* getCard(const Letter& letter, const Number& number);

    // Replaces the card pointer stored at the requested board position.
    void setCard(const Letter& letter, const Number& number, Card* card);

    // Prints the board followed by all players in insertion order.
    friend std::ostream& operator<<(std::ostream& os, const Game& game);

private:
    Board board;
    std::vector<Player> players;
    int round;
    const Card* previousCard;
    const Card* currentCard;
};

