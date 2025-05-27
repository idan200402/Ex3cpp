#pragma once
#include "Player.hpp"
#include <stdexcept>

namespace ex3 {
    /**
     * @class PlayerTest
     * @brief A test player class that inherits from Player.
     * This class is used for tets purposes only and does not implement any specific abilities.
     * I made this class to test the player class without any modifications to the base methods.
     */
    class PlayerTest: public Player{
        public:
        //using the same constructor as Player.
            using Player::Player;
            /**
             * @brief Main ability of the test player.
             * This method is not implemented and will throw an exception if called.
             * @param target The player that the ability will be applied to.
             * @throws std::runtime_error if called.
             */
            virtual void mainAbility(Player* target)override;
            /**
             * @brief Secondary ability of the test player.
             * This method is not implemented and will throw an exception if called.
             * @param target The player that the ability will be applied to.
             * @throws std::runtime_error if called.
             */
            virtual void secondaryAbility(Player* target) override;
    };
}