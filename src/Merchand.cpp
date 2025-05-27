#include "../include/Merchand.hpp"
#include "../include/Game.hpp"
#include <stdexcept>

namespace ex3 {
    // Constructor for Merchand class
    Merchand::Merchand(Game& game, const std::string& name)
        : Player(game, name) {
        this->role = "Merchand";
    }

    void Merchand::onStartTurn() {
        //calling the Player's onStartTurn method to handle general start of turn logic.
        Player::onStartTurn();
        // If the Merchand has at least 3 coins and the last move was not a coup, add 1 coin to the player.
         if(coins >= 3 && lastMove !="coup") {
            addCoins(1);
        }
    }
    void Merchand::mainAbility(Player* /*target*/) {
        throw std::runtime_error("Merchand does not have a main ability");
    }
    void Merchand::secondaryAbility(Player* /*target*/) {
        throw std::runtime_error("Merchand does not have a secondary ability");
    }
}