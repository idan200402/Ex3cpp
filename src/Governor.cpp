#include "Governor.hpp"
#include "Game.hpp"
#include <stdexcept>

namespace ex3 {
    Governor::Governor(Game& game, const std::string& name)
        : Player(game, name) {
        this->role = "Governor"; 
    }
    void Governor::gather() {
        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("It's not your turn");
        }
        if (sanctioned) {
            throw std::runtime_error("You are sanctioned");
        }
        addCoins(1);
        lastMove = "gather";
        lastTarget = "";
        game.nextTurn();
    }
    void Governor::tax() {
        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("It's not your turn");
        }
        if (sanctioned) {
            throw std::runtime_error("You are sanctioned");
        }
        addCoins(3);
        lastMove = "tax";
        lastTarget = "";
        //add here more implementation for the governor to be able to cancel the tax for others.
        game.nextTurn();
    }
    void Governor::bribe() {
        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("It's not your turn");
        }
        if(coins < 4) {
            throw std::runtime_error("Not enough coins to pay bribe");
        }
        if (sanctioned) {
            throw std::runtime_error("You are sanctioned");
        }
        removeCoins(4);
        lastMove = "bribe";
        lastTarget = "";
        //need to implement the bribing logic
        game.nextTurn();
    }
    void Governor::arrest(Player& target) {
        throw std::runtime_error("Governor cannot arrest");
    }
    void Governor::sanction(Player& target) {
        throw std::runtime_error("Governor cannot sanction");
    }
    void Governor::coup(Player& target) {
        if(!game.isPlayerTurn(this)) {
            throw std::runtime_error("It's not your turn");
        }
        if (coins < 7) {
            throw std::runtime_error("Not enough coins to coup");
        }
        if (sanctioned) {
            throw std::runtime_error("You are sanctioned");
        }
        removeCoins(7);
        lastMove = "coup";
        lastTarget = target.getName();
        game.removePlayer(&target);
        game.nextTurn();
    }
    void Governor::undo(Player& target) {
        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("It's not your turn");
        }
        if (sanctioned) {
            throw std::runtime_error("You are sanctioned");
        }
        lastMove = "undo";
        lastTarget = target.getName();
        game.nextTurn();
    }
}




    