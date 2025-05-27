#pragma once
#include "Player.hpp"

namespace ex3 {
    /**
     * @class Baron
     * @brief Represents a Baron player in the game.
     * The Baron can invest coins and he recieves a coin when sanctioned.
     */
    class Baron : public Player {
    private:
        /**
         * @brief Invests 3 coins to get 6 coins.
         * @throws std::runtime_error if it's not the player's turn or if the player does not have enough coins.
         */   
        void invest();
        /**
         * @brief Defines the behavior when the Baron is sanctioned.
         * If the Baron is sanctioned while performing a financial action, he receives 1 coin.
         * @param actionName The name of the action that triggered the sanction.
         */
        void whenSanctioned(const std::string& actionName) override;
    public:
        /**
         * @brief Constructor for the Baron class.
         * Initializes the Baron with a name and associates it with a game instance , also with his role.
         * @param game Reference to the Game instance this player belongs to.
         * @param name The name of the player.
         */
        Baron(Game& game, const std::string& name);
        /**
         * @brief Barons main ability is to invest coins.
         * @param target  , baron's main ability does not require a target , so wont use it.
         */
        void mainAbility(Player* target) override;
        /**
         * @brief Barons secondary ability is not defined in this context.
         * Throws an exception if called.
         * @param target , not used in this context.
         * @throws std::runtime_error if called.
         */
        void secondaryAbility(Player* target) override;

    };
}