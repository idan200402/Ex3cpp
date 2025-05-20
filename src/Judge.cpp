#include "Judge.hpp"
#include "Game.hpp"
#include <stdexcept>

namespace ex3 {
    Judge::Judge(Game& game, const std::string& name)
        : Player(game, name) {
        this->role = "Judge";
    }

    void Judge::blockBribe(Player& target) {
        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("It's not your turn");
        }
        blockBribed[target.getName()] = true;
    }
}