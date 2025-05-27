// idan.shumski@gmail.com

#pragma once
#include "Player.hpp"
#include <unordered_map>

namespace ex3 {
    /**
     * @class Governor
     * @brief Represents a Governor player in the game.
     * The Governor can enable tax for other players and also he receives 3 coins when taxing.
     */

    class Governor : public Player {
    private:
    // two unordered maps to hold the enabled tax for players and the turn counter when they were enabled.
        std::unordered_map<std::string, bool> enabledTax;
        std::unordered_map<std::string, int> indexOfEnabledTax;
        /**
         * @brief Enables tax for a target player.
         * @param target The player for whom tax is enabled.
         * @throws std::runtime_error if it's not the player's turn.
         */
         std::unordered_map<std::string, bool>& getEnabledTax();
         /**
          * @brief Enables tax for a target player.
          * @param target The player for whom tax is enabled.
          * @throws std::runtime_error if it's not the player's turn.
          */
         void undo(Player& target);
    public:
        /**
         * @brief Constructor for the Governor class.
         * Initializes the Governor with a name and associates it with a game instance, also with his role.
         * @param game Reference to the Game instance this player belongs to.
         * @param name The name of the player.
         */
        Governor(Game& game, const std::string& name);
        /**
         * @brief If the Governor is not sanctioned, he recieves 3 coins.
         * @throws std::runtime_error if it's not the player's turn or if the player is sanctioned.
         */
        void tax() override;
        /**
         * @brief Called at the start of each turn for this player.
         * It expires the enabled tax for players that were enabled in previous turns.
         */
        void onStartTurn() override;
        /**
         * @brief The Governor's main ability is to enable tax for a target player.
         * Calls the private method undo.
         * @param target The player for whom tax is enabled.
         */
        void mainAbility(Player* target) override;
        /**
         * @brief The Governor's secondary ability is not defined in this context.
         * Throws an exception if called.
         * @param target , not used in this context.
         * @throws std::runtime_error if called.
         */
        void secondaryAbility(Player* target) override;
        /**
         * @brief Gets the list of players that this Governor has enabled tax for.
         * @return A reference to an unordered map of player names and their status.
         */
        std::unordered_map<std::string, bool>& getList() override; 
    };

}
