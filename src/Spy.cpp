#include "Spy.hpp"
#include "Game.hpp"
#include <iostream>
#include <stdexcept>

namespace ex3 {
    Spy::Spy(Game& game, const std::string& name)
        : Player(game, name) {
        this->role = "Spy";
    }

    void Spy::
    }