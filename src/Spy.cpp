#include "../include/Spy.hpp"
#include "../include/Game.hpp"
#include <iostream>
#include <stdexcept>

namespace ex3 {
    Spy::Spy(Game& game, const std::string& name)
        : Player(game, name) {
        this->role = "Spy";
        this->arrestdisabled = {};
        this->indexOfArrestDisabled = {};
        this->blockArrestCount = 0;
        this->inspectedCoins = 0;
    }
    void Spy::inspectCoins(Player& target) {
        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("It's not your turn");
        }
        lastMove = "inspect";
        lastTarget = target.getName();
        this->inspectedCoins = target.getCoinsCount();
    }
    void Spy::blockArrest(Player& target) {
        if (blockArrestCount == 1) {
            game.nextTurn();
            throw std::runtime_error("You have allready blocked in this turn");
        }
        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("It's not your turn");
        }
        lastMove = "blockArrest";
        lastTarget = target.getName();
        arrestdisabled[target.getName()] = true;
        blockArrestCount++;
    }
    std::unordered_map<std::string, bool>& Spy::getArrestDisabled() {
        return arrestdisabled;
    }
    std::unordered_map<std::string, bool>& Spy::getList() {
        return getArrestDisabled();
    }
    void Spy::onStartTurn() {
        int currentTurn = game.getTurnCounter();
        for(auto it = arrestdisabled.begin(); it != arrestdisabled.end();) {
            const std::string& playerName = it->first;
            if(indexOfArrestDisabled.count(playerName)>0 && indexOfArrestDisabled[playerName] < currentTurn) {
                it = arrestdisabled.erase(it);
                indexOfArrestDisabled.erase(playerName);
            } else {
                ++it;
            } 
        }
        Player::onStartTurn();
    }
    void Spy::mainAbility(Player* target) {
        if (!target) {
            throw std::runtime_error("Target cannot be null");
        }
        inspectCoins(*target);
    }
    void Spy::secondaryAbility(Player* target) {
        if (!target) {
            throw std::runtime_error("Target cannot be null");
        }
        blockArrest(*target);
    }
        
}