#include "../include/Merchand.hpp"
#include "../include/Game.hpp"
#include <stdexcept>

namespace ex3 {
    Merchand::Merchand(Game& game, const std::string& name)
        : Player(game, name) {
        this->role = "Merchand";
    }

    void Merchand::onStartTurn() {
        PlayerTest::onStartTurn();
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