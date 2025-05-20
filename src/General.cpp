#include "General.hpp"
#include "Game.hpp"
#include <stdexcept>

namespace ex3 {
    General::General(Game& game, const std::string& name)
        : Player(game, name) {
        this->role = "General";
    }

    void General::saveFromCoup(Player& target) {
        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("It's not your turn");
        }
        if (this->coins < 5) {
            throw std::runtime_error("Not enough coins to save from couping");
        }
        this->removeCoins(5);
        savedFromCoup[target.getName()] = true;
    }
    std::unordered_map<std::string, bool>& General::getSavedFromCoup() {
        return savedFromCoup;
    }
}
        