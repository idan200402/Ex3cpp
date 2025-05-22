#include "../include/Merchand.hpp"
#include "../include/Game.hpp"
#include <stdexcept>

namespace ex3 {
    Merchand::Merchand(Game& game, const std::string& name)
        : Player(game, name) {
        this->role = "Merchand";
    }

    void Merchand::onStartTurn() {
        Player::onStartTurn();
         if(coins >= 3 && lastMove !="coup") {
            addCoins(1);
         }
    }
}