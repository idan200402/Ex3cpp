// idan.shumski@gmail.com

#include "Player.hpp"
#include "Game.hpp"
#include <stdexcept>

namespace ex3 {

    Player::Player(Game& game, const std::string& name)
        : game(game),
          name(name),
          role(""),
          coins(0),
          alive(true),
          lastMove(""),
          lastTarget(""),
          sanctioned(false)
    {
        game.addPlayer(this);
    }

    const std::string& Player::getName() const {
        return name;
    }

    const std::string& Player::getRole() const {
        return role;
    }

    int Player::getCoinsCount() const {
        return coins;
    }

    bool Player::isAlive() const {
        return alive;
    }

    const std::string& Player::getLastMove() const {
        return lastMove;
    }

    const std::string& Player::getLastTarget() const {
        return lastTarget;
    }

    bool Player::isSanctioned() const {
        return sanctioned;
    }

    void Player::addCoins(int amount) {
        if (amount < 0) {
            throw std::invalid_argument("Amount must be non-negative");
        }
        coins += amount;
    }

    void Player::removeCoins(int amount) {
        if (amount < 0) {
            throw std::invalid_argument("Amount must be non-negative");
        }
        if (coins < amount) {
            throw std::runtime_error("Not enough coins");
        }
        coins -= amount;
    }

    void Player::deactivate() {
        alive = false;
    }

    void Player::markSanctioned(bool status) {
        sanctioned = status;
    }
}
