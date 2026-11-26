#include "Rules.h"

#include "Card.h"
#include "Types.h"

#include <stdexcept>

Rules::Rules() : nextPlayerIndex(0) {}

bool Rules::isValid(const Game& game) const {
    const Card* currentCard = game.getCurrentCard();
    if (currentCard == nullptr) {
        return false;
    }

    const Card* previousCard = game.getPreviousCard();
    if (previousCard == nullptr) {
        return true;
    }

    const bool sameAnimal = static_cast<FaceAnimal>(*previousCard) == static_cast<FaceAnimal>(*currentCard);
    const bool sameBackground = static_cast<FaceBackground>(*previousCard) == static_cast<FaceBackground>(*currentCard);

    return sameAnimal || sameBackground;
}

bool Rules::gameOver(const Game& game) const {
    return game.getRound() >= 7;
}

bool Rules::roundOver(const Game& game) const {
    int activePlayers = 0;

    for (const Player& player : game.getPlayers()) {
        if (player.isActive()) {
            ++activePlayers;
        }
    }

    return activePlayers == 1;
}

const Player& Rules::getNextPlayer(const Game& game) {
    const std::vector<Player>& players = game.getPlayers();
    const int numberOfPlayers = static_cast<int>(players.size());

    if (numberOfPlayers == 0) {
        throw std::logic_error("Cannot select a player from an empty game.");
    }
    
    for (int i = 0; i < numberOfPlayers; ++i) {
        const int currPlayerIndex = nextPlayerIndex % numberOfPlayers;
        nextPlayerIndex = (currPlayerIndex + 1) % numberOfPlayers;

        if (players[currPlayerIndex].isActive()) {
            return players[currPlayerIndex];
        }
    }

    throw std::logic_error("Cannot select a player when no players are active.");
}

void Rules::resetTurnOrder() {
    nextPlayerIndex = 0;
}

