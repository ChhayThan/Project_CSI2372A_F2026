#include "Player.h"

#include "Rubis.h"
#include "Types.h"

#include <ostream>

Player::Player(const std::string& name, Side side)
    : name(name), side(side), active(true), rubies(0), endOfGame(false) {}

std::string Player::getName() const {
    return name;
}

void Player::setActive(bool isActive) {
    active = isActive;
}

bool Player::isActive() const {
    return active;
}

int Player::getNRubies() const {
    return rubies;
}

void Player::addRubis(const Rubis& rubisCard) {
    rubies += static_cast<int>(rubisCard);
}

void Player::setDisplayMode(bool showEndOfGameScore) {
    endOfGame = showEndOfGameScore;
}

Side Player::getSide() const {
    return side;
}

void Player::setSide(Side newSide) {
    side = newSide;
}

std::ostream& operator<<(std::ostream& os, const Player& player) {
    os << player.name << ": ";

    if (player.endOfGame) {
        os << player.rubies << (player.rubies == 1 ? " ruby" : " rubies");
        return os;
    }

    switch (player.side) {
        case Side::Top:
            os << "top";
            break;
        case Side::Bottom:
            os << "bottom";
            break;
        case Side::Left:
            os << "left";
            break;
        case Side::Right:
            os << "right";
            break;
    }

    os << (player.active ? " (active)" : " (inactive)");
    return os;
}

