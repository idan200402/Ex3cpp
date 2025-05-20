#include "Merchand.hpp"
#include "Game.hpp"
#include <stdexcept>

namespace ex3 {
    Merchand::Merchand(Game& game, const std::string& name)
        : Player(game, name) {
        this->role = "Merchand";
    }

    void Merchand::onStartTurn() {
         if(coins >= 3) {
            addCoins(1);
         }
    }
    