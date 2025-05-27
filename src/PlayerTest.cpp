#include "../include/PlayerTest.hpp"
#include <stdexcept>

namespace ex3 {

    void PlayerTest::mainAbility(Player* target) {
        throw std::runtime_error("Test player does not have a main ability");
    }
    void PlayerTest::secondaryAbility(Player* target) {
        throw std::runtime_error("Test player does not have a secondary ability");
    }
}