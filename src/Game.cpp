#include "../include/Game.hpp"
#include "../include/Player.hpp"
#include <stdexcept>
#include "../include/General.hpp"

namespace ex3 {
    Game::Game() : shuttingDown(false) ,turnCounter(0),currentPlayerIndex(0){
        this->players.reserve(6); 
    }

    void Game::addPlayer(Player* player) {
        if (players.size() >= 6) {
            throw std::out_of_range("Cannot add more than 6 players");
        }
        for(Player* p : players) {
            if (p->getName() == player->getName()) {
                throw std::invalid_argument("Player with this name already exists");
            }
        }
        players.push_back(player);
    }

    std::vector<Player*> Game::getPlayers() const {
        std::vector<Player*> activePlayers;
        for(Player* player : players) {
            if (player->isAlive()) {
                activePlayers.push_back(player);
            }
        }
        return activePlayers;  
    }
    std::string Game::turn() const {
        if (getActivePlayerCount() == 0) {
            throw std::runtime_error("No active players");
        }
        int index = currentPlayerIndex % players.size();
        while(!players[index]->isAlive()) {
            index = (index + 1) % players.size();
        }
        return players[index]->getName();
    }
    std::string Game::getWinner() const {
        if(getActivePlayerCount() == 0) {
            throw std::runtime_error("No players in the game");
        }
        if(getActivePlayerCount() > 1) {
            throw std::runtime_error("Game is still ongoing");
        }
        else{
            for(Player* player : players) {
                if(player->isAlive()) {
                    return player->getName();
                }
            }
        }
        return ""; // This line should never be reached
    }
    Game::~Game() {
        shuttingDown = true;
        for (Player* player : players) {
            delete player; 
        }
    }
    int Game::getTurnCounter() const {
        return turnCounter;
    }
    void Game::nextTurn() {
    if (getActivePlayerCount() <= 1) {
        return; // No more turns if only one player is left
    }

    Player* currentPlayer = players[currentPlayerIndex];

    // If current player has a bonus turn (from bribe), use it now without advancing
    if (currentPlayer->hasNextTurn) {
        currentPlayer->hasNextTurn = false;
        return;
    }

    // Advance to the next alive player
    do {
        currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
    } while (!players[currentPlayerIndex]->isAlive());

    currentPlayer = players[currentPlayerIndex];

    // 💡 Clear coup protection (General) if this player was protected
    for (Player* player : players) {
        if (player->getRole() == "General") {
            auto general = dynamic_cast<General*>(player);
            if (general) {
                auto& savedMap = general->getSavedFromCoup();
                auto it = savedMap.find(currentPlayer->getName());
                if (it != savedMap.end()) {
                    it->second = false; // protection expires when turn begins
                }
            }
        }
    }

    turnCounter++;
    currentPlayer->onStartTurn(); // optional per-role behavior
}

    bool Game::isPlayerTurn(Player* p) const {
        return players[currentPlayerIndex] == p;
    }
    void Game::removePlayer(Player* p) {
        p->deactivate();
    }
    int Game::getActivePlayerCount() const {
        int count = 0;
        for (Player* player : players) {
            if (player->isAlive()) {
                count++;
            }
        }
        return count;
    }
    bool Game::isShuttingDown() const {
        return shuttingDown;
    }
    bool Game::isPlayerAlive(Player& p) const {
        for (Player* player : players) {
            if (player == &p) {
                return player->isAlive();
            }
        }
    }
    void Game::forceTurnTo(Player* player) {
    for (size_t i = 0; i < players.size(); ++i) {
        if (players[i] == player && player->isAlive()) {
            currentPlayerIndex = i;
            return;
        }
    }
    throw std::runtime_error("Cannot force turn to a non-existent or dead player");
}

}
        
        
        
        