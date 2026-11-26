#include "CardDeck.h"
#include "Game.h"
#include "Player.h"
#include "Rules.h"
#include "RubisDeck.h"

#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

// Removes leading and trailing whitespace from user-entered text.
std::string trim(const std::string& text) {
    const std::string whitespace = " \t\r\n";
    const std::string::size_type first = text.find_first_not_of(whitespace);

    if (first == std::string::npos) {
        return "";
    }

    const std::string::size_type last = text.find_last_not_of(whitespace);
    return text.substr(first, last - first + 1);
}

// Reads a player count from two through four, retrying after invalid input.
int readPlayerCount(std::istream& input, std::ostream& output) {
    std::string line;

    while (true) {
        output << "Enter the number of players (2-4): ";

        if (!std::getline(input, line)) {
            throw std::runtime_error("Input ended before the player count was entered.");
        }

        std::istringstream parser(line);
        int playerCount = 0;
        char extraCharacter = '\0';

        if ((parser >> playerCount) && !(parser >> extraCharacter) &&
            playerCount >= 2 && playerCount <= 4) {
            return playerCount;
        }

        output << "Invalid player count. Please enter 2, 3, or 4.\n";
    }
}

// Reads and trims a non-empty player name, retrying after blank input.
std::string readPlayerName(
    std::istream& input,
    std::ostream& output,
    int playerNumber
) {
    std::string name;

    while (true) {
        output << "Enter the name of player " << playerNumber << ": ";

        if (!std::getline(input, name)) {
            throw std::runtime_error("Input ended before all player names were entered.");
        }

        name = trim(name);
        if (!name.empty()) {
            return name;
        }

        output << "Player name cannot be empty.\n";
    }
}

// Assigns opposite sides for two players and distinct sides for larger games.
std::vector<Side> getPlayerSides(int playerCount) {
    if (playerCount == 2) {
        return {Side::top, Side::bottom};
    }

    if (playerCount == 3) {
        return {Side::top, Side::left, Side::right};
    }

    return {Side::top, Side::bottom, Side::left, Side::right};
}

// Identifies one card position on the Board.
struct Position {
    Letter letter;
    Number number;
};

// Returns the three cards directly in front of the requested board side.
std::vector<Position> getPreviewPositions(Side side) {
    switch (side) {
        case Side::top:
            return {
                {Letter::A, Number::Two},
                {Letter::A, Number::Three},
                {Letter::A, Number::Four}
            };
        case Side::bottom:
            return {
                {Letter::E, Number::Two},
                {Letter::E, Number::Three},
                {Letter::E, Number::Four}
            };
        case Side::left:
            return {
                {Letter::B, Number::One},
                {Letter::C, Number::One},
                {Letter::D, Number::One}
            };
        case Side::right:
            return {
                {Letter::B, Number::Five},
                {Letter::C, Number::Five},
                {Letter::D, Number::Five}
            };
    }

    throw std::logic_error("Player has an invalid board side.");
}

// Waits for the user to press Enter and reports an unexpected end of input.
void waitForEnter(std::istream& input) {
    std::string line;
    if (!std::getline(input, line)) {
        throw std::runtime_error("Input ended during the private card preview.");
    }
}

// Resets all state required at the beginning of a round.
void initializeRound(Game& game, Rules& rules) {
    game.allFacesDown();
    game.setCurrentCard(nullptr);
    rules.resetTurnOrder();

    for (const Player& player : game.getPlayers()) {
        game.getPlayer(player.getSide()).setActive(true);
    }
}

// Reveals and then hides the three cards directly in front of one player.
void showPrivatePreview(
    Game& game,
    const Player& player,
    std::istream& input,
    std::ostream& output
) {
    const std::vector<Position> positions = getPreviewPositions(player.getSide());

    output << "\n" << player.getName()
           << ", ask the other players to look away, then press Enter to view your cards.";
    waitForEnter(input);

    for (const Position& position : positions) {
        game.turnFaceUp(position.letter, position.number);
    }

    output << "\n" << game;
    output << "Press Enter when you are finished viewing your cards.";
    waitForEnter(input);

    for (const Position& position : positions) {
        game.turnFaceDown(position.letter, position.number);
    }

    // Move the previous preview out of immediate view before the next player.
    output << std::string(25, '\n');
}

// Gives every player their private preview in insertion order.
void showPrivatePreviews(
    Game& game,
    std::istream& input,
    std::ostream& output
) {
    for (const Player& player : game.getPlayers()) {
        showPrivatePreview(game, player, input, output);
    }
}

} // namespace

int main() {
    try {
        std::cout << "Memoarrr! - Base Game Setup\n\n";

        const int playerCount = readPlayerCount(std::cin, std::cout);
        std::vector<std::string> playerNames;
        playerNames.reserve(playerCount);

        for (int playerNumber = 1; playerNumber <= playerCount; ++playerNumber) {
            playerNames.push_back(readPlayerName(std::cin, std::cout, playerNumber));
        }

        // Construct and shuffle both singleton decks before gameplay begins.
        CardDeck::make_CardDeck();
        RubisDeck::make_RubisDeck();

        Game game;
        const std::vector<Side> playerSides = getPlayerSides(playerCount);

        for (int index = 0; index < playerCount; ++index) {
            game.addPlayer(Player(playerNames[index], playerSides[index]));
        }

        Rules rules;
        initializeRound(game, rules);

        std::cout << "\nBase game created successfully.\n";
        std::cout << "Preparing round " << game.getRound() + 1 << ".\n";
        showPrivatePreviews(game, std::cin, std::cout);

        std::cout << "Round " << game.getRound() + 1 << " is ready.\n\n";
        std::cout << game;
    } catch (const std::exception& error) {
        std::cerr << "Unable to create the game: " << error.what() << '\n';
        return 1;
    }

    return 0;
}

