#include "../include/Game.hpp"
//#include "../include/Player.hpp"
#include <stdexcept>
#include "../include/General.hpp"
#include <unordered_map>

namespace ex3 {
    //Constructor for the Game class
    Game::Game() : shuttingDown(false) ,turnCounter(0),currentPlayerIndex(0){
        this->players.reserve(6); 
    }

    void Game::addPlayer(Player* player) {
        // Check if the size of players exceeds the maximum limit of 6
        if (players.size() >= 6) {
            throw std::out_of_range("Cannot add more than 6 players");
        }
        // Check if a player with the same name already exists
        for(Player* p : players) {
            if (p->getName() == player->getName()) {
                throw std::invalid_argument("Player with this name already exists");
            }
        }
        // Add the player to the players vector
        players.push_back(player);
    }

    std::vector<Player*> Game::getPlayers() const {
        // Return a vector of pointers to active players (those who are alive)
        std::vector<Player*> activePlayers;
        for(Player* player : players) {
            if (player->isAlive()) {
                activePlayers.push_back(player);
            }
        }
        return activePlayers;  
    }
    std::string Game::turn() const {
        //checks if there are any active players, if not throws an error.
        if (getActivePlayerCount() == 0) {
            throw std::runtime_error("No active players");
        }
        //calculates the index of the current player based on the currentPlayerIndex and the size of players vector.
        int index = currentPlayerIndex % players.size();
        while(!players[index]->isAlive()) {
            index = (index + 1) % players.size();
        }
        return players[index]->getName();
    }
    std::string Game::getWinner() const {
        //checks if there are any active players, if not throws an error.
        if(getActivePlayerCount() == 0) {
            throw std::runtime_error("No players in the game");
        }
        // If the game is still ongoing, return an error
        if(getActivePlayerCount() > 1) {
            throw std::runtime_error("Game is still ongoing");
        }
        else{
            //iterates through the players vector to find the only player who is alive and returns their name.
            for(Player* player : players) {
                if(player->isAlive()) {
                    return player->getName();
                }
            }
        }
        return ""; // This line should never be reached
    }

    int Game::getTurnCounter() const {
        return turnCounter;
    }
    void Game::nextTurn() {
        if (getActivePlayerCount() <= 1) {
            return; // No more turns if only one player is left
        }
        // getting the current player based on the currentPlayerIndex
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

        //Clear coup protection (General) if this player was protected
        for (Player* player : players) {
            if (player->getRole() == "General") {
                auto general = dynamic_cast<General*>(player);
                //checking that the dynamic_cast was successful
                if (general) {
                    auto& savedMap = general->getList();
                    auto it = savedMap.find(currentPlayer->getName());
                    if (it != savedMap.end()) {
                        it->second = false; // protection expires when turn begins
                    }
                }
            }
        }
        //now we can increment the turn counter and call the onStartTurn method of the current player.
        turnCounter++;
        currentPlayer->onStartTurn(); // optional per-role behavior
    }
    //
    bool Game::isPlayerTurn(Player* p) const {
        // Check the player is this index is the parameter p.
        return players[currentPlayerIndex] == p;
    }
    void Game::removePlayer(Player* p) {
        //making the player inactive by calling the deactivate method.
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
        // find the player and return their alive status
        for (Player* player : players) {
            if (player == &p) {
                return player->isAlive();
            }
        }
    }
    void Game::forceTurnTo(Player* player) {
        // Check if the player exists in the game and is alive
        for (size_t i = 0; i < players.size(); ++i) {
            if (players[i] == player && player->isAlive()) {
                // Set the current player index to this player's index
                currentPlayerIndex = i;
                return;
            }
        }
        // If the player does not exist or is dead, throw an error
        throw std::runtime_error("Cannot force turn to a non-existent or dead player");
    }
    Game::~Game() {
        shuttingDown = true; // Set the shutting down flag to true
        for (Player* player : players) {
            delete player; // Delete each player to free memory
        }
        players.clear(); // Clear the players vector
    }

}
        
        
        
        