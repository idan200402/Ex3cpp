#pragma once
#include "Player.hpp"
 
namespace ex3 {
    /**
     * @class Merchand
     * @brief Represents a Merchand player in the game.
     * The Merchand can gain coins at the start of each turn if they have at least 3 coins.
     * Also if he is arrested he will pay 2 coins to the game bank instead of 1 coin to the player who arrested him.
     */
    class Merchand : public Player{
        public:
            /**
             * @brief Constructor for the Merchand class.
             * Initializes the Merchand with a name and associates it with a game instance, also with his role.
             * @param game Reference to the Game instance this player belongs to.
             * @param name The name of the player.
             */
            Merchand(Game& game, const std::string& name);
            /**
             * @brief Called at the start of each turn for this player.
             * If the Merchand has at least 3 coins and the last move was not a coup, it will add 1 coin to the player.
             */
            void onStartTurn() override;
            /**
             * @brief The Merchand's main ability is not defined in this context.
             * Throws an exception if called.
             * @param target , not used in this context.
             * @throws std::runtime_error if called.
             */
            void mainAbility(Player* target) override;
            /**
             * @brief The Merchand's secondary ability is not defined in this context.
             * Throws an exception if called.
             * @param target , not used in this context.
             * @throws std::runtime_error if called.
             */
            void secondaryAbility(Player* target) override;
    };
}