#include "../include/General.hpp"
#include "../include/Game.hpp"
#include <stdexcept>

namespace ex3 {
    // Constructor for General class
    General::General(Game& game, const std::string& name)
        : Player(game, name) {
        this->role = "General";
        this->savedFromCoup = {};
        this->indexOfSavedFromCoup = {};
    }

    void General::saveFromCoup(Player& target) {
        // If it's not the player's turn, throw an error.
        if (!game.isPlayerTurn(this)) {
           throw std::runtime_error("It's not your turn");
        }
        // If the player does not have enough coins, throw an error.
        if (this->coins < 5) {
            throw std::runtime_error("Not enough coins to save from couping");
        }
        //updating the last move and the maps.
        this->removeCoins(5);
        savedFromCoup[target.getName()] = true;
        indexOfSavedFromCoup[target.getName()] = game.getTurnCounter();
    }
    //calling the private method getsavedFromCoup to return the unordered map of players saved from coup.
    std::unordered_map<std::string, bool>& General::getList() {
        return getSavedFromCoup();
    }
    std::unordered_map<std::string, bool>& General::getSavedFromCoup() {
        return savedFromCoup;
    }
    void General::onStartTurn() {
        for (auto it = savedFromCoup.begin(); it != savedFromCoup.end(); ) {
            const std::string& playerName = it->first;
            
            // If the player being saved is the one whose turn it is now, expire protection
            if (game.turn() == playerName) {
                it = savedFromCoup.erase(it);
                indexOfSavedFromCoup.erase(playerName);
            } else {
                ++it;
            }
        }
        // Call the Player's onStartTurn method to handle any other start of turn logic.
        Player::onStartTurn();
    }
    void General::mainAbility(Player* target) {
        //check if the target is not null before proceeding.
        if (!target) {
            throw std::runtime_error("Target cannot be null");
        }
        // Call the private method saveFromCoup to save the target player from coup.
        saveFromCoup(*target);
    }
    void General::secondaryAbility(Player* /*target*/) {
        throw std::runtime_error("General does not have a secondary ability");

    }
}
        