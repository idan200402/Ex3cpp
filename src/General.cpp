#include "../include/General.hpp"
#include "../include/Game.hpp"
#include <stdexcept>

namespace ex3 {
    General::General(Game& game, const std::string& name)
        : Player(game, name) {
        this->role = "General";
        this->savedFromCoup = {};
        this->indexOfSavedFromCoup = {};
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
        indexOfSavedFromCoup[target.getName()] = game.getTurnCounter();
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

    Player::onStartTurn();
}

}
        