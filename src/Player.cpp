// idan.shumski@gmail.com

#include "Player.hpp"
#include "Game.hpp"
#include <stdexcept>
#include "Governor.hpp"
#include "Spy.hpp"


namespace ex3 {

    Player::Player(Game& game, const std::string& name)
        : game(game),
          name(name),
          coins(0),
          role(""),
          alive(true),
          lastMove(""),
          lastTarget(""),
          sanctioned(false),
          hasNextTurn(false)
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

    bool Player::isSanctioned() {
        bool currentSanctioned = sanctioned;
        this->sanctioned = false; // Reset the sanctioned status after checking
        return currentSanctioned;
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
    void Player::gather() {
        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("It's not your turn");
        }
        if (isSanctioned()) {
            whenSanctioned("gather");
            throw std::runtime_error("You are sanctioned");
        }
        addCoins(1);
        lastMove = "gather";
        lastTarget = "";
        game.nextTurn();
    }
    void Player::tax() {
        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("It's not your turn");
        }
        bool isEnabled = false;
        for(auto& player : game.getPlayers()) {
            if (player->getRole() == "Governor") {
                auto governor = dynamic_cast<Governor*>(player);
                if (governor == nullptr) {
                    throw std::runtime_error("Player is not a Governor or the dinamic_cast failed");
                }
                auto Isfound = governor->getEnabledTax().find(this->getName());
                if (Isfound != governor->getEnabledTax().end() && Isfound->second == true) {
                    Isfound->second = false;
                    isEnabled = true;
                }
            }
        }
        if (isEnabled) {
            game.nextTurn();
            whenSanctioned("tax");
            throw std::runtime_error("At least one Governor has enabled tax");
        }   
        addCoins(3);
        lastMove = "tax";
        lastTarget = "";
        game.nextTurn();
    }
    void Player::bribe() {
        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("It's not your turn");
        }
        if(lastMove == "bribe") {
            throw std::runtime_error("You cannot bribe twice in a row");
        }
        if (coins < 4) {
            throw std::runtime_error("Not enough coins to bribe");
        }
        removeCoins(4);
        hasNextTurn = true;
        lastMove = "bribe";
        lastTarget = "";
        game.nextTurn();
    }
    void Player::arrest(Player& target) {

        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("It's not your turn");
        }
        bool isBlocked = false;
        for(auto& player : game.getPlayers()) {
            if (player->getRole() == "Spy") {
                auto spy = dynamic_cast<Spy*>(player);
                if (spy == nullptr) {
                    throw std::runtime_error("Player is not a Spy or the dinamic_cast failed");
                }
                auto Isfound = spy->getArrestDisabled().find(target.getName());
                if (Isfound != spy->getArrestDisabled().end() && Isfound->second == true) {
                    Isfound->second = false;
                    isBlocked = true;

                }
            }
        }
        if (isBlocked) {
            game.nextTurn();
            throw std::runtime_error("Arrest is blocked by a Spy");
        }
        if(lastMove == "arrest" && lastTarget == target.getName()) {
            throw std::runtime_error("You cannot arrest the same player twice in a row");
        }
        lastMove = "arrest";
        lastTarget = target.getName();
        game.removePlayer(&target);
        game.nextTurn();
    }
    void Player::sanction(Player& target) {
        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("It's not your turn");
        }
        lastMove = "sanction";
        lastTarget = target.getName();
        target.markSanctioned(true);
        game.nextTurn();
    }
    void Player::coup(Player& target) {
        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("It's not your turn");
        }
        if (coins < 7) {
            throw std::runtime_error("Not enough coins to coup");
        }
        removeCoins(7);
        lastMove = "coup";
        lastTarget = target.getName();
        game.removePlayer(&target);
        game.nextTurn();
    }
        
}       
