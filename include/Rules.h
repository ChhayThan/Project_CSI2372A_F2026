#pragma once

#include "Game.h"

// Evaluates card matches, game progress, round progress, and player turns.
class Rules {
public:
    // Creates rules with turn selection starting at the first player.
    Rules();

    // Returns whether the current card is a valid continuation of the turn.
    bool isValid(const Game& game) const;

    // Returns whether seven rounds have been completed.
    bool gameOver(const Game& game) const;

    // Returns whether exactly one player remains active in the round.
    bool roundOver(const Game& game) const;

    // Returns the next active player and advances the cyclic turn index.
    const Player& getNextPlayer(const Game& game);

    // Restarts turn selection at the first player.
    void resetTurnOrder();

private:
    int nextPlayerIndex;
};

