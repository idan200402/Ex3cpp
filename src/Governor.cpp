#include "Governor.hpp"
#include "Game.hpp"
#include <stdexcept>

namespace ex3 {
    Governor::Governor(Game& game, const std::string& name)
        : Player(game, name) {
        this->role = "Governor";
        this->enabledTax = {};
        this->indexOfEnabledTax = {};
    }

    void Governor::tax() {
        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("It's not your turn");
        }
        if (isSanctioned()) {
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
    std::unordered_map<std::string, bool>& Governor::getEnabledTax()  {
        return enabledTax;
    }
    void Governor::onStartTurn() {
        int currectTurn = game.getTurnCounter();
        for(auto it = enabledTax.begin(); it != enabledTax.end();) {
            const std::string& playerName = it->first;
            if(indexOfEnabledTax.count(playerName)>0 && indexOfEnabledTax[playerName] < currectTurn) {
                it = enabledTax.erase(it);
                indexOfEnabledTax.erase(playerName);
            } else {
                ++it;
            } 
        }
        Player::onStartTurn();
    }

}