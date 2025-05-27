#pragma once 
#include "Player.hpp"
#include <unordered_map>
 namespace ex3 {
    /**
     * @class Spy
     * @brief Represents a Spy player in the game.
     * The Spy can inspect the coins of other players and block their arrest.
     */
    class Spy : public Player {
    private: 
        // Counter for the number of times the Spy has blocked an arrest in the current turn
        int blockArrestCount;
        // Two unordered maps to hold the players who have been arrested and the turn counter when they were arrested.
        std::unordered_map<std::string, bool> arrestdisabled;
        std::unordered_map<std::string, int> indexOfArrestDisabled;
        /**
         * @brief Inspects the coins of a target player.
         * @param target The player whose coins are being inspected.
         * @throws std::runtime_error if it's not the player's turn.
         */
        void inspectCoins(Player& target);
        /**
         * @brief Blocks the arrest of a target player.
         * @param target The player whose arrest is being blocked.
         * @throws std::runtime_error if it's not the player's turn or if the Spy has already blocked an arrest in this turn.
         */
        void blockArrest(Player& target); 
        /**
         * @brief Gets the list of players who have been arrested by this Spy.
         * @return A reference to an unordered map of player names and their arrested status.
         */
        std::unordered_map<std::string, bool>& getArrestDisabled();
    public:
        /**
         * @brief Constructor for the Spy class.
         * Initializes the Spy with a name and associates it with a game instance, also with his role.
         * @param game Reference to the Game instance this player belongs to.
         * @param name The name of the player.
         */
        Spy(Game& game, const std::string& name);
        /**
         * @brief Called at the start of each turn for this player.
         * It expires the blocked arrest status for players that were blocked in previous turns.
         */
        void onStartTurn() override;
        /**
         * @brief The Spy's main ability is to inspect the coins of a target player.
         * calls the private method inspectCoins.
         * @param target The player whose coins are being inspected.
         * 
         */
        void mainAbility(Player* target) override;
        /**
         * @brief The Spy's secondary ability is to block the arrest of a target player.
         * calls the private method blockArrest.
         * @param target The player whose arrest is being blocked.
         * @throws std::runtime_error if called when it's not the Spy's turn or if the Spy has already blocked an arrest in this turn.
         */
        void secondaryAbility(Player* target) override;
        /**
         * @brief Gets the list of players that this Spy has blocked from arrest.
         * @return A reference to an unordered map of player names and their blocked status.
         */
        std::unordered_map<std::string, bool>& getList() override;
    };
}
        