// idan.shumski@gmail.com

#include <iostream>
#include "Game.hpp"
#include "Governor.hpp"
#include <stdexcept>

using namespace ex3;

int main() {
    try {
        Game game;

        // Create two Governors
        Governor* g1 = new Governor(game, "Alice");
        Governor* g2 = new Governor(game, "Bob");

        std::cout << "Initial players:" << std::endl;
        for (const std::string& name : game.getPlayers()) {
            std::cout << "- " << name << std::endl;
        }

        std::cout << "\nCurrent turn: " << game.turn() << std::endl;

        // Simulate some actions
        if (game.isPlayerTurn(g1)) {
            g1->tax(); // Alice should get 3 coins
        }

        std::cout << g1->getName() << " has " << g1->getCoinsCount() << " coins\n";
        std::cout << "Next turn: " << game.turn() << std::endl;

        if (game.isPlayerTurn(g2)) {
            g2->gather(); // Bob gets 1 coin
        }

        std::cout << g2->getName() << " has " << g2->getCoinsCount() << " coins\n";
        std::cout << "Next turn: " << game.turn() << std::endl;

        // Cleanup
        delete g1;
        delete g2;

    } catch(const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
