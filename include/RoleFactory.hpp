#pragma once
#include <string>
#include "Game.hpp"
#include "Player.hpp"

namespace ex3 {
    class RoleFactory{
        public:
            static Player* createRole(Game& game, const std::string& name , const std::string& role);
    };
}