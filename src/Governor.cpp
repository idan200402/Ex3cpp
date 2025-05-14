#include "Governor.hpp"
#include "Game.hpp"
#include <stdexcept>

namespace ex3 {
    Governor::Governor(Game& game, const std::string& name)
        : Player(game, name) {
        this->role = "Governor";
        this->enabledTax = {};
    }

    void Governor::tax() {
        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("It's not your turn");
        }
        if (sanctioned) {
            throw std::runtime_error("You are sanctioned");
        }
        addCoins(3);
        lastMove = "tax";
        lastTarget = "";
        game.nextTurn();
    }
    void Governor::undo(Player& target) {
        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("It's not your turn");
        }
        enabledTax[target.getName()] = true;
    }
    std::unordered_map<std::string, bool>& Governor::getEnabledTax() const {
        return enabledTax;
    }

}