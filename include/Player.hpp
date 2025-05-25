#pragma once
#include "PlayerTest.hpp"
#include <stdexcept>
#include <unordered_map>

namespace ex3 {

    class Player: public PlayerTest {
        public:
            using PlayerTest::PlayerTest;
            virtual ~Player() = default;
            virtual void mainAbility(Player* target) = 0;
            virtual void secondaryAbility(Player* target) = 0;
            virtual std::unordered_map<std::string, bool>& getList(){
                throw std::runtime_error(this->name + " does not have a list");
            }
    };
}