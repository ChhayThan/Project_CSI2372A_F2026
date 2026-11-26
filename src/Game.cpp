#include "Game.h"

#include <ostream>
#include <stdexcept>

Game::Game()
    : board(), players(), round(0), previousCard(nullptr), currentCard(nullptr) {
    players.reserve(4);
}

int Game::getRound() const {
    return round;
}

void Game::setRound(int newRound) {
    if (newRound < 0 || newRound > 7) {
        throw std::out_of_range("Round must be between 0 and 7.");
    }

    round = newRound;
}

void Game::addPlayer(const Player& player) {
    if (players.size() >= 4) {
        throw std::length_error("A game cannot have more than four players.");
    }

    for (const Player& existingPlayer : players) {
        if (existingPlayer.getSide() == player.getSide()) {
            throw std::invalid_argument("Each player must have a unique board side.");
        }
    }

    players.push_back(player);
}

Player& Game::getPlayer(Side side) {
    for (Player& player : players) {
        if (player.getSide() == side) {
            return player;
        }
    }

    throw std::out_of_range("No player is assigned to the requested side.");
}

const Player& Game::getPlayer(Side side) const {
    for (const Player& player : players) {
        if (player.getSide() == side) {
            return player;
        }
    }

    throw std::out_of_range("No player is assigned to the requested side.");
}

const std::vector<Player>& Game::getPlayers() const {
    return players;
}

const Card* Game::getPreviousCard() const {
    return previousCard;
}

const Card* Game::getCurrentCard() const {
    return currentCard;
}

void Game::setCurrentCard(const Card* card) {
    if (card == nullptr) {
        previousCard = nullptr;
        currentCard = nullptr;
        return;
    }

    previousCard = currentCard;
    currentCard = card;
}

bool Game::isFaceUp(const Letter& letter, const Number& number) const {
    return board.isFaceUp(letter, number);
}

bool Game::turnFaceUp(const Letter& letter, const Number& number) {
    return board.turnFaceUp(letter, number);
}

bool Game::turnFaceDown(const Letter& letter, const Number& number) {
    return board.turnFaceDown(letter, number);
}

void Game::allFacesDown() {
    board.allFacesDown();
}

Card* Game::getCard(const Letter& letter, const Number& number) {
    return board.getCard(letter, number);
}

void Game::setCard(const Letter& letter, const Number& number, Card* card) {
    board.setCard(letter, number, card);
}

std::ostream& operator<<(std::ostream& os, const Game& game) {
    os << game.board;

    for (const Player& player : game.players) {
        os << player << '\n';
    }

    return os;
}

