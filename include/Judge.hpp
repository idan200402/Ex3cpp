#pragma once
#include "Player.hpp"
#include <unordered_map>

namespace ex3 {
    /**
     * @class Judge
     * @brief Represents a Judge player in the game.
     * The Judge can block bribing actions from other players, and has a list of players who have been blocked.
     * Also when sanctioned, the Player who sanctioned the Judge will pay 1 coin to the game bank.
     */
    class Judge : public Player {
        private:
            // Two unordered maps to hold the players who have been blocked from bribing and the turn counter when they were blocked.
            std::unordered_map<std::string, bool> blockBribed;
            std::unordered_map<std::string, int> indexOfBlockBribed;
            /**
            * @brief Blocks a target player from bribing.
            * @param target The player to be blocked from bribing.
            * @throws std::runtime_error if it's not the player's turn.
            */
            void blockBribe(Player& target);
            /**
             * @brief Gets the list of players who have been blocked from bribing.
             * @return A reference to an unordered map of player names and their blocked status.
             */
             std::unordered_map<std::string, bool>& getBlockBribed();
        public:
            /**
             * @brief Constructor for the Judge class.
             * Initializes the Judge with a name and associates it with a game instance, also with his role.
             * @param game Reference to the Game instance this player belongs to.
             * @param name The name of the player.
             */
            Judge(Game& game, const std::string& name);
            /**
             * @brief Called at the start of each turn for this player.
             * It expires the blocked bribing status for players that were blocked in previous turns.
             */
            void onStartTurn() override;
            /**
             * @brief The Judge's main ability is to block a target player from bribing.
             * Calls the private method blockBribe.
             * @param target The player to be blocked from bribing.
             */
            void mainAbility(Player* target) override;
            /**
             * @brief The Judge's secondary ability is not defined in this context.
             * Throws an exception if called.
             * @param target , not used in this context.
             * @throws std::runtime_error if called.
             */
            void secondaryAbility(Player* target) override;
            /**
             * @brief Gets the list of players that this Judge has blocked from bribing.
             * @return A reference to an unordered map of player names and their blocked status.
             */
            std::unordered_map<std::string, bool>& getList() override;
    };
}