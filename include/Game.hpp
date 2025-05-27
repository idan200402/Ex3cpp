//idan.shumski@gmail.com
#pragma once
#include <string>
#include <vector>

namespace ex3 {
    // Forward declaration of Player class to avoid circular dependency
    class Player;
    /**
     * @class Game
     * @brief Represents the game state, managing players , turns, and game logic.
     * This class handles the game flow, including adding players, managing turns, and determining the winner.
     */

    class Game {
    // Member variables

    private:
        // flag that is false until calling ~Game()
        bool shuttingDown;
        // Counter for the number of turns taken in the game        
        int turnCounter;
        // Index of the current player in the players vector
        int currentPlayerIndex;
        // Vector to hold pointers to Player objects
        std::vector<Player*> players;
    public:
        /**
         * @brief Constructor for the Game class.
         * Initializes the game state, including the players vector and other member variables.
         */
        Game();
        /**
         * @brief Adds a player to the game.
         * @param player Pointer to the Player object to be added.
         * @throws std::out_of_range if the maximum number of players (6) is exceeded.
         * @throws std::invalid_argument if a player with the same name already exists.
         */
        void addPlayer(Player* player);
        /**
         * @brief Retrieves the list of active players in the game.
         * @return A vector of pointers to Player objects that are currently alive.
         */
        std::vector<Player*> getPlayers() const;
        /**
         * @brief Gets the name of the player whose turn it is.
         * @return The name of the current player.
         * @throws std::runtime_error if there are no active players.
         */
        std::string turn() const;
        /**
         * @brief Determines the winner of the game.
         * @return The name of the winning player.
         * @throws std::runtime_error if there are no players or if the game is still ongoing.
         */
        std::string getWinner() const;
        /**
         * @brief Advances the game to the next player's turn.
         * This method updates the current player index and handles any necessary game state changes.
         */
        void nextTurn();
        /**
         * @brief Checks if it is the specified player's turn.
         * @param p Pointer to the Player object to check.
         * @return true if it is the player's turn, false otherwise.
         */
        bool isPlayerTurn(Player* p) const;
        /**
         * @brief Removes a player from the game.
         * Marks the player as inactive and removes them from the game state.
         * @param p Pointer to the Player object to be removed.
         */
        void removePlayer(Player* p);
        /**
         * @brief Gets the number of active players in the game.
         * @return The count of players who are currently alive.
         */
        int getActivePlayerCount() const;
        /**
         * @brief Gets the current turn counter.
         * @return The number of turns that have been taken in the game.
         */
        int getTurnCounter() const;
        /**
         * @brief Destructor for the Game class.
         * deletes all Player objects and sets the shuttingDown flag to true.
         */
        ~Game();
        /**
         * @brief Checks if the game is in the process of shutting down.
         * @return true if the game is shutting down, false otherwise.
         */
        bool isShuttingDown() const;
        /**
         * @brief Checks if the specified player is alive.
         * @param p Reference to the Player object to check.
         * @return true if the player is alive, false otherwise.
         */
        bool isPlayerAlive(Player& p) const;
        /**
         * @brief Forces the turn to the specified player.
         * This method sets the current player index to the specified player's index, if they are alive.
         * @param player Pointer to the Player object to force the turn to.
         * @throws std::runtime_error if the player does not exist or is dead.
         */
        void forceTurnTo(Player* player);
      
    };  
}
        