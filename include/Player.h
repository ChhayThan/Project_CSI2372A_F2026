#pragma once

#include "Types.h"

#include <iosfwd>
#include <string>

class Rubis;
enum class Side {
    top,
    bottom,
    left,
    right
};

// Stores one player's identity, board position, round status, and ruby score.
class Player {
public:
    // Creates an active player with no rubies.
    Player(const std::string& name, Side side);

    // Returns the player's name.
    std::string getName() const;

    // Changes whether the player is active in the current round.
    void setActive(bool active);

    // Returns true when the player is active in the current round.
    bool isActive() const;

    // Returns the total number of rubies won by the player.
    int getNRubies() const;

    // Adds the value of a ruby card to the player's score.
    void addRubis(const Rubis& rubis);

    // Selects regular status output or end-of-game score output.
    void setDisplayMode(bool endOfGame);

    // Returns the player's side of the board.
    Side getSide() const;

    // Changes the player's side of the board.
    void setSide(Side side);

    // Prints the player's round status or final ruby score.
    friend std::ostream& operator<<(std::ostream& os, const Player& player);

private:
    std::string name;
    Side side;
    bool active;
    int rubies;
    bool endOfGame;
};

