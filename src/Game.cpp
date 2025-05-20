#include "Game.hpp"
#include "Player.hpp"
#include <stdexcept>

namespace ex3 {
    Game::Game() : currentPlayerIndex(0){}

    void Game::addPlayer(Player* player) {
        if (players.size() >= 6) {
            throw std::out_of_range("Cannot add more than 6 players");
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
    }
    void Game::nextTurn() {
        if (getActivePlayerCount() <= 1) {
            return; // No more turns if only one player is left
        }
        Player* currentPlayer = players[currentPlayerIndex];
        if (currentPlayer->hasNextTurn) {
            currentPlayer->hasNextTurn = false;
            return; // Skip to the next player
        }
        do{
            currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
        } while(!players[currentPlayerIndex]->isAlive());
        currentPlayer->onStartTurn();
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
}
        
        
        
        