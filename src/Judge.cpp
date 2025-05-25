#include "../include/Judge.hpp"
#include "../include/Game.hpp"
#include <stdexcept>

namespace ex3 {
    Judge::Judge(Game& game, const std::string& name)
        : Player(game, name) {
        this->role = "Judge";
        this->blockBribed = {};
        this->indexOfBlockBribed = {};
    }

    void Judge::blockBribe(Player& target) {
        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("It's not your turn");
        }
        blockBribed[target.getName()] = true;
    }
    std::unordered_map<std::string, bool>& Judge::getBlockBribed() {
        return blockBribed;
    }
    void Judge::onStartTurn() {
        int currectTurn = game.getTurnCounter();
        for(auto it = blockBribed.begin(); it != blockBribed.end();) {
            const std::string& playerName = it->first;
            if(indexOfBlockBribed.count(playerName)>0 && indexOfBlockBribed[playerName] < currectTurn) {
                it = blockBribed.erase(it);
                indexOfBlockBribed.erase(playerName);
            } else {
                ++it;
            } 
        }
        Player::onStartTurn();
    }
    std::unordered_map<std::string, bool>& Judge::getList() {
        return getBlockBribed();
    }
    void Judge::mainAbility(Player* target) {
        if (!target) {
            throw std::runtime_error("Target cannot be null");
        }
        blockBribe(*target);
    }
    void Judge::secondaryAbility(Player* /*target*/) {
        throw std::runtime_error("Judge does not have a secondary ability");
    }
}