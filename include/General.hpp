//idan.shumski@gmail.com
#pragma once
#include "Player.hpp"
#include <unordered_map>

namespace ex3 {
    /**
     * @class General
     * @brief Represents a General player in the game.
     * The General can save players from being couped thus he has a list of players saved from coup.
     * Also if he is arrested he will recieve the coins that the player who arrested him has taken from him.
     */
    class General : public Player {
        private:
            //two unordered maps to hold the players saved from coup and the turn counter when they were saved.
            std::unordered_map<std::string, bool> savedFromCoup;
            std::unordered_map<std::string, int> indexOfSavedFromCoup;

            std::unordered_map<std::string, bool>& getSavedFromCoup();
            /**
             * @brief Saves a target player from being couped by paying 5 coins.
             * @param target The player to be saved from coup.
             * @throws std::runtime_error if it's not the player's turn or if the player does not have enough coins.
             */
            void saveFromCoup(Player& target);
        public:
            /**
             * @brief Constructor for the General class.
             * Initializes the General with a name and associates it with a game instance, also with his role.
             * @param game Reference to the Game instance this player belongs to.
             * @param name The name of the player.
             */
            General(Game& game, const std::string& name);
            /**
             * @brief the General's main ability is to save a player from being couped.
             * calles the private method saveFromCoup.
             * @param target The player to be saved from coup.
             */
            void mainAbility(Player* target) override;
            /**
             * @brief the General's secondary ability is not defined in this context.
             * Throws an exception if called.
             * @param target , not used in this context.
             * @throws std::runtime_error if called.
             */
            void secondaryAbility(Player* target) override; 
            /**
             * @brief Called at the start of each turn for this player.
             * It expires the protection from coup for players saved by this General.
             * If the player being saved is the one whose turn it is now, expire protection.
             */
            void onStartTurn() override;
            /**
             * @brief Gets the list of players that this General has saved from coup.
             * @return A reference to an unordered map of player names and their status.
             */
            std::unordered_map<std::string, bool>& getList() override;
    };
}
            