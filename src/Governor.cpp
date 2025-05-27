#include "../include/Governor.hpp"
#include "../include/Game.hpp"
#include <stdexcept>

namespace ex3 {
    // Constructor for Governor class
    Governor::Governor(Game& game, const std::string& name)
        : Player(game, name) {
        this->role = "Governor";
        this->enabledTax = {};
        this->indexOfEnabledTax = {};
    }
    
    void Governor::tax() {
        // If it's not the player's turn or if the player is sanctioned, throw an error.
        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("It's not your turn");
        }
        if (isSanctioned()) {
            throw std::runtime_error("You are sanctioned");
        }
        //instead of adding 2 coins, the Governor adds 3 coins to himself.
        addCoins(3);
        lastMove = "tax";
        lastTarget = "";
        game.nextTurn();
    }
    void Governor::undo(Player& target) {
        //if its not the player's turn, throw an error.
        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("It's not your turn");
        }
        //updating the map with the enabled tax for the target player and the turn counter when it was enabled.
        enabledTax[target.getName()] = true;
    }
    std::unordered_map<std::string, bool>& Governor::getEnabledTax()  {
        return enabledTax;
    }
    void Governor::onStartTurn() {
        // Iterate through the enabledTax map and remove players whose tax has expired.
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
    void Governor::mainAbility(Player* target) {
        // Check if the target is not null before proceeding.
        if (!target) {
            throw std::runtime_error("Target cannot be null");
        }
        // Call the private method undo to enable tax for the target player.
        undo(*target);
    }
    void Governor::secondaryAbility(Player* /*target*/) {
        throw std::runtime_error("Governor does not have a secondary ability");
    }
    std::unordered_map<std::string, bool>& Governor::getList() {
        return getEnabledTax();
    }
}