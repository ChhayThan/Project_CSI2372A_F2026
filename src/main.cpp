#include "CardDeck.h"
#include "Game.h"
#include "Player.h"
#include "Rules.h"
#include "RubisDeck.h"

#include <algorithm>
#include <cctype>
#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace input {

// Identifies one card position on the Board.
struct Position {
    Letter letter;
    Number number;
};

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

// Converts a coordinate such as A1 or d4 into its Board enumerations.
bool parsePosition(const std::string& text, Position& position) {
    const std::string coordinate = trim(text);
    if (coordinate.size() != 2) {
        return false;
    }

    const char row = static_cast<char>(
        std::toupper(static_cast<unsigned char>(coordinate[0]))
    );
    const char column = coordinate[1];

    if (row < 'A' || row > 'E' || column < '1' || column > '5') {
        return false;
    }

    position.letter = static_cast<Letter>(row - 'A');
    position.number = static_cast<Number>(column - '1');
    return true;
}

// Reads a valid, face-down, non-center Board position.
Position readFaceDownPosition(
    const Game& game,
    std::istream& input,
    std::ostream& output
) {
    std::string line;

    while (true) {
        output << "Select a face-down card (A1-E5): ";

        if (!std::getline(input, line)) {
            throw std::runtime_error("Input ended before a card was selected.");
        }

        Position position{};
        if (!parsePosition(line, position)) {
            output << "Invalid coordinate. Enter a row A-E followed by a column 1-5.\n";
            continue;
        }

        if (position.letter == Letter::C && position.number == Number::Three) {
            output << "C3 is the empty center and cannot be selected.\n";
            continue;
        }

        if (game.isFaceUp(position.letter, position.number)) {
            output << "That card is already face up. Choose another card.\n";
            continue;
        }

        return position;
    }
}

} // namespace input

namespace {

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

        name = input::trim(name);
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

// Returns the three cards directly in front of the requested board side.
std::vector<input::Position> getPreviewPositions(Side side) {
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
    const std::vector<input::Position> positions = getPreviewPositions(player.getSide());

    output << "\n" << player.getName()
           << ", ask the other players to look away, then press Enter to view your cards.";
    waitForEnter(input);

    for (const input::Position& position : positions) {
        game.turnFaceUp(position.letter, position.number);
    }

    output << "\n" << game;
    output << "Press Enter when you are finished viewing your cards.";
    waitForEnter(input);

    for (const input::Position& position : positions) {
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

// Returns true when at least one non-center card remains face down.
bool hasFaceDownCard(const Game& game) {
    const Letter letters[] = {
        Letter::A, Letter::B, Letter::C, Letter::D, Letter::E
    };
    const Number numbers[] = {
        Number::One,
        Number::Two,
        Number::Three,
        Number::Four,
        Number::Five
    };

    for (Letter letter : letters) {
        for (Number number : numbers) {
            if (letter == Letter::C && number == Number::Three) {
                continue;
            }

            if (!game.isFaceUp(letter, number)) {
                return true;
            }
        }
    }

    return false;
}

// Returns the sole active player after a round has ended.
Player& getRoundWinner(Game& game) {
    for (const Player& player : game.getPlayers()) {
        if (player.isActive()) {
            return game.getPlayer(player.getSide());
        }
    }

    throw std::logic_error("Round ended without an active player.");
}

// Runs base-game turns until only one player remains active.
Player& playRound(
    Game& game,
    Rules& rules,
    std::istream& inputStream,
    std::ostream& output
) {
    while (!rules.roundOver(game)) {
        const Player& selectedPlayer = rules.getNextPlayer(game);
        const Side selectedSide = selectedPlayer.getSide();

        output << "\n" << selectedPlayer.getName() << "'s turn.\n";

        if (!hasFaceDownCard(game)) {
            output << "No face-down cards remain. " << selectedPlayer.getName()
                   << " is out of this round.\n";
            game.getPlayer(selectedSide).setActive(false);
            output << game;
            continue;
        }

        const input::Position position =
            input::readFaceDownPosition(game, inputStream, output);

        game.turnFaceUp(position.letter, position.number);
        game.setCurrentCard(game.getCard(position.letter, position.number));

        if (rules.isValid(game)) {
            output << "The selected card is valid.\n";
        } else {
            output << "The selected card does not match. "
                   << selectedPlayer.getName() << " is out of this round.\n";
            game.getPlayer(selectedSide).setActive(false);
        }

        output << game;
    }

    return getRoundWinner(game);
}

// Awards the next ruby card to a round winner and advances the round count.
void awardRoundWinner(
    Game& game,
    Player& winner,
    RubisDeck& rubisDeck,
    std::ostream& output
) {
    Rubis* reward = rubisDeck.getNext();
    if (reward == nullptr) {
        throw std::logic_error("Ruby deck was exhausted before all rounds were completed.");
    }

    const int completedRound = game.getRound() + 1;
    winner.addRubis(*reward);
    game.setRound(completedRound);

    const int rubyCount = static_cast<int>(*reward);
    output << winner.getName() << " wins round " << completedRound
           << " and receives " << rubyCount
           << (rubyCount == 1 ? " ruby.\n" : " rubies.\n");
}

// Prints a list of tied winner names.
void printWinnerNames(
    const std::vector<Player>& winners,
    std::ostream& output
) {
    for (std::vector<Player>::size_type index = 0; index < winners.size(); ++index) {
        if (index > 0) {
            output << (index + 1 == winners.size() ? " and " : ", ");
        }
        output << winners[index].getName();
    }
}

// Prints final scores from least to most rubies and announces every top scorer.
void displayFinalResults(const Game& game, std::ostream& output) {
    std::vector<Player> rankings = game.getPlayers();
    if (rankings.empty()) {
        throw std::logic_error("Cannot display results for a game without players.");
    }

    for (Player& player : rankings) {
        player.setDisplayMode(true);
    }

    std::stable_sort(
        rankings.begin(),
        rankings.end(),
        [](const Player& left, const Player& right) {
            return left.getNRubies() < right.getNRubies();
        }
    );

    output << "\nFinal scores (least to most rubies):\n";
    for (const Player& player : rankings) {
        output << player << '\n';
    }

    const int winningScore = rankings.back().getNRubies();
    std::vector<Player> winners;
    for (const Player& player : rankings) {
        if (player.getNRubies() == winningScore) {
            winners.push_back(player);
        }
    }

    output << (winners.size() == 1 ? "Winner: " : "Winners: ");
    printWinnerNames(winners, output);
    output << " with " << winningScore
           << (winningScore == 1 ? " ruby.\n" : " rubies.\n");
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
        RubisDeck& rubisDeck = RubisDeck::make_RubisDeck();

        Game game;
        const std::vector<Side> playerSides = getPlayerSides(playerCount);

        for (int index = 0; index < playerCount; ++index) {
            game.addPlayer(Player(playerNames[index], playerSides[index]));
        }

        std::cout << "\nBase game created successfully.\n";
        Rules rules;

        while (!rules.gameOver(game)) {
            initializeRound(game, rules);

            std::cout << "\nPreparing round " << game.getRound() + 1 << ".\n";
            showPrivatePreviews(game, std::cin, std::cout);

            std::cout << "Round " << game.getRound() + 1 << " is ready.\n\n";
            std::cout << game;

            Player& roundWinner = playRound(game, rules, std::cin, std::cout);
            std::cout << '\n';
            awardRoundWinner(game, roundWinner, rubisDeck, std::cout);
        }

        displayFinalResults(game, std::cout);
    } catch (const std::exception& error) {
        std::cerr << "Unable to create the game: " << error.what() << '\n';
        return 1;
    }

    return 0;
}

