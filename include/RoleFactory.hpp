#pragma once
#include <string>
#include "Game.hpp"
#include "Player.hpp"

namespace ex3 {
    /**
     * @class RoleFactory
     * @brief Factory class for creating Player objects with specific roles.
     * This class provides a static method to create players based on their roles.
     */
    class RoleFactory{
        public:
        /**
         * @brief Creates a Player object with the specified role.
         * @param game Reference to the Game instance this player belongs to.
         * @param name The name of the player.
         * @param role The role of the player (e.g., "Baron", "General", etc.).
         * @return A pointer to the created Player object.
         * @throws std::invalid_argument if the specified role does not exist.
         */
            static Player* createRole(Game& game, const std::string& name , const std::string& role);
    };
}