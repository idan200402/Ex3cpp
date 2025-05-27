// idan.shumski@gmail.com

#pragma once
#include <string>
#include "../include/Game.hpp"
#include <unordered_map>
#include <stdexcept>

namespace ex3 {
    
    /**
     * @class Player abstract base class.
     * @brief Represents a player in the game.
     * This class manages player attributes, actions, and interactions with the game.
     */
    class Player {
    protected:
        // Reference to the game instance this player belongs to
        Game& game;
        // Player attributes
        std::string name;
        std::string role;
        int coins;
        bool alive;
        std::string lastMove;
        std::string lastTarget;
        bool sanctioned;
        /**
         * @brief Indicates whether the player has a next turn.
         * This is used to determine if the player can perform actions in the next turn.
         */
        bool hasNextTurn;
        /**
         * @brief each role has its own behavior when sanctioned.
         */
        virtual void whenSanctioned(const std::string& /*actionName*/) {}
        
    public:
        /**
         * @brief Called at the start of each turn for this player.
         * This method can be overridden by derived classes to implement specific behavior.
         */
        virtual void onStartTurn();
        
        /**
         * @brief Constructor for the Player class.
         * Initializes the player with a name and associates it with a game instance.
         * @param game Reference to the Game instance this player belongs to.
         * @param name The name of the player.
         * @throws std::invalid_argument if the name is empty.
         */
        Player(Game& game, const std::string& name);
        //getter methods.
        const std::string& getName() const;
        const std::string& getRole() const;
        int getCoinsCount() const;
        bool isAlive() const;
        const std::string& getLastMove() const;
        const std::string& getLastTarget() const;
        bool isSanctioned() ;
        /**
         * @brief If the player is not sanctioned it will add 1 coin to the player.
         * @throws std::runtime_error if it's not the player's turn or if the player is sanctioned.
         */
        virtual void gather();
        /**
         * @brief If the player is not sanctioned it will add 2 coins to the player.
         * @throws std::runtime_error if it's not the player's turn or if the player is sanctioned.
         * @throws std::runtime_error if at least one Governor has enabled tax.
         * @throws std::runtime_error if the dynamic_cast to Governor fails.
         */
        virtual void tax();
        /**
         * @brief The player can pay 4 coins to gain an extra action in current turn.
         * @throws std::runtime_error if it's not the player's turn or if the player bribed prior.
         * @throws std::runtime_error if the player does not have enough coins.
         * @throws std::runtime_error if the dynamic_cast to Judge fails.
         * @throws std::runtime_error if the bribe was blocked by a Judge.
         */
        virtual void bribe();
        /**
         * @brief Arrests a target player, taking a coin from them.
         * @param target The player to be arrested.
         * @throws std::runtime_error if it's not the player's turn or if the player is not alive.
         * @throws std::runtime_error if a spy blocks the arrest for this player.
         * @throws std::runtime_error if the dynamic_cast to Spy fails.
         * @throws std::runtime_error if the player tries to arrest the same target twice in a row.
         */
        virtual void arrest(Player & target);
        /**
         * @brief Sanctions a target player, removing 3 coins from this player and marking the target as sanctioned.
         * it will enable the target to make a financial action on his upcoming turn.
         * @param target The player to be sanctioned.
         * @throws std::runtime_error if it's not the player's .
         * @throws std::runtime_error if the player does not have enough coins.
         */
        virtual void sanction(Player& target);
        /**
         * @brief Coup a target player, removing 7 coins from this player and killing the target.
         * @param target The player to be couped.
         * @throws std::runtime_error if it's not the player's turn or if the player does not have enough coins.
         * @throws std::runtime_error if the dynamic_cast to General fails.
         * @throws std::runtime_error if the target is being protected by a General.
         */
        virtual void coup(Player& target);
        /**
         * @brief add coins to the player.
         * @param amount The amount of coins to add.
         */
        void addCoins(int amount);
        /**
         * @brief Removes coins from the player.
         * @param amount The amount of coins to remove.
         * @throws std::runtime_error if the player does not have enough coins.
         */
        void removeCoins(int amount);
        /**
         * @brief turns the player into inactive.
         */
        void deactivate();
        void markSanctioned(bool status);

        virtual ~Player();
        /**
         * @brief a pure virtual function that must be implemented by derived classes.
         * This function defines the main ability of the player.
         * @param target The player that the ability will be applied to.
         */
        virtual void mainAbility(Player* target) = 0;
        /**
         * @brief a pure virtual function that must be implemented by derived classes.
         * This function defines the secondary ability of the player.
         * @param target The player that the ability will be applied to.
         */
        virtual void secondaryAbility(Player* target) = 0;
        /**
         * @brief Gets the list of players that has in common something.
         * @return A reference to an unordered map of player names and their status.
         */
        virtual std::unordered_map<std::string, bool>& getList() {
            throw std::runtime_error(this->name + " does not have a list");
        }
    };
}
