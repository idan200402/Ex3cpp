#include "../include/Judge.hpp"
#include "../include/Game.hpp"
#include <stdexcept>

namespace ex3 {
    // Constructor for Judge class
    Judge::Judge(Game& game, const std::string& name)
        : Player(game, name) {
        this->role = "Judge";
        this->blockBribed = {};
        this->indexOfBlockBribed = {};
    }

    void Judge::blockBribe(Player& target) {
        // If it's not the player's turn, throw an error.
        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("It's not your turn");
        }
        //updating the map with the blocked bribing for the target player .
        blockBribed[target.getName()] = true;
    }
    std::unordered_map<std::string, bool>& Judge::getBlockBribed() {
        return blockBribed;
    }
    void Judge::onStartTurn() {
        // Iterate through the blockBribed map and remove players whose bribing block has expired.
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
        // Check if the target is not null before proceeding.
        if (!target) {
            throw std::runtime_error("Target cannot be null");
        }
        // Call the private method blockBribe to block the target player from bribing.
        blockBribe(*target);
    }
    // The Judge does not have a secondary ability defined in this context.
    void Judge::secondaryAbility(Player* /*target*/) {
        throw std::runtime_error("Judge does not have a secondary ability");
    }
}