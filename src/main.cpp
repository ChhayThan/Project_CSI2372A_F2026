#include "CardDeck.h"
#include "Game.h"
#include "Player.h"
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

        std::cout << "\nBase game created successfully.\n\n";
        std::cout << game;
    } catch (const std::exception& error) {
        std::cerr << "Unable to create the game: " << error.what() << '\n';
        return 1;
    }

    return 0;
}

