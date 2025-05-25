// idan.shumski@gmail.com

#include "../include/Player.hpp"
#include "../include/Game.hpp"
#include <stdexcept>
#include "../include/Governor.hpp"
#include "../include/Spy.hpp"
#include <iostream>
#include "../include/General.hpp"
#include "../include/Judge.hpp"

namespace ex3 {

    PlayerTest::PlayerTest(Game& game, const std::string& name)
        : game(game),
          name(name),
          role(""),
          coins(0),
          alive(true),
          lastMove(""),
          lastTarget(""),
          sanctioned(false),
          hasNextTurn(false)
    {
        if (name.empty()) {
            throw std::invalid_argument("Name cannot be empty");
        }
        //std::cout << "Created player: " << name << " with coins = " << coins << std::endl;
        game.addPlayer(this);
    }

    const std::string& PlayerTest::getName() const {
        return name;
    }

    const std::string& PlayerTest::getRole() const {
        return role;
    }

    int PlayerTest::getCoinsCount() const {
        return coins;
    }

    bool PlayerTest::isAlive() const {
        return alive;
    }

    const std::string& PlayerTest::getLastMove() const {
        return lastMove;
    }

    const std::string& PlayerTest::getLastTarget() const {
        return lastTarget;
    }
    PlayerTest::~PlayerTest() {
        //no need to delete game, as it is not owned by this class
    }

    bool PlayerTest::isSanctioned() {
        bool currentSanctioned = sanctioned;
        this->sanctioned = false; // Reset the sanctioned status after checking
        return currentSanctioned;
    }

    void PlayerTest::addCoins(int amount) {
        if (amount < 0) {
            throw std::invalid_argument("Amount must be non-negative");
        }
        coins += amount;
    }

    void PlayerTest::removeCoins(int amount) {
        if (amount < 0) {
            throw std::invalid_argument("Amount must be non-negative");
        }
        if (coins < amount) {
            throw std::runtime_error("Not enough coins");
        }
        coins -= amount;
    }
    void PlayerTest::onStartTurn() {
        if(game.isShuttingDown()) {
            throw std::runtime_error("Game is shutting down");
        }
        //std::cout << this->getName() << " is starting their turn with " << this->getCoinsCount() << " coins." << std::endl;
        if(coins >= 10) {
            for(auto& player : game.getPlayers()) {
                if (player!= this &&player->isAlive()) {
                    this->coup(*player);
                    break;
                }
            }
        }
    }

    void PlayerTest::deactivate() {
        alive = false;
    }

    void PlayerTest::markSanctioned(bool status) {
        sanctioned = status;
    }
    void PlayerTest::gather() {
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
    void PlayerTest::tax() {
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
                auto Isfound = governor->getList().find(this->getName());
                if (Isfound != governor->getList().end() && Isfound->second == true) {
                    Isfound->second = false;
                    isEnabled = true;
                }
            }
        }
        if (isEnabled) {
            game.nextTurn();
            //whenSanctioned("tax");
            throw std::runtime_error("At least one Governor has enabled tax");
        }   
        addCoins(3);
        lastMove = "tax";
        lastTarget = "";
        game.nextTurn();
    }
   void PlayerTest::bribe() {
    if (!game.isPlayerTurn(this)) {
        throw std::runtime_error("It's not your turn");
    }
    if (lastMove == "bribe") {
        throw std::runtime_error("You cannot bribe twice in a row");
    }
    if (coins < 4) {
        throw std::runtime_error("Not enough coins to bribe");
    }
    removeCoins(4);

    bool isBlocked = false;
    for (auto& player : game.getPlayers()) {
        if (player->getRole() == "Judge") {
            auto judge = dynamic_cast<Judge*>(player);
            if (judge == nullptr) {
                throw std::runtime_error("Player is not a Judge or the dynamic_cast failed");
            }
            auto Isfound = judge->getList().find(this->getName());
            if (Isfound != judge->getList().end() && Isfound->second == true) {
                Isfound->second = false;
                isBlocked = true;
            }
        }
    }

    lastMove = "bribe";
    lastTarget = "";

    if (!isBlocked) {
        hasNextTurn = true;
    } else {
       throw std::runtime_error("Bribe was blocked by a Judge");
    }

    
}


    void PlayerTest::arrest(PlayerTest& target) {

        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("It's not your turn");
        }
        if(game.isPlayerAlive(target) == false) {
            throw std::runtime_error("Target is not alive");
        }
        
        bool isBlocked = false;
        for(auto& player : game.getPlayers()) {
            if (player->getRole() == "Spy") {
                auto spy = dynamic_cast<Spy*>(player);
                if (spy == nullptr) {
                    throw std::runtime_error("Player is not a Spy or the dinamic_cast failed");
                }
                auto Isfound = spy->getList().find(target.getName());
                if (Isfound != spy->getList().end() && Isfound->second == true) {
                    Isfound->second = false;
                    isBlocked = true;

                }
            }
        }
        if (isBlocked) {
            game.nextTurn();
            std::cout << target.getName() << " was blocked from being arrested by a Spy" << std::endl;
        }
        if(lastMove == "arrest" && lastTarget == target.getName()) {
            throw std::runtime_error("You cannot arrest the same player twice in a row");
        }
        lastMove = "arrest";
        lastTarget = target.getName();
        if(target.getRole() == "General") {
            addCoins(1);
            game.nextTurn();
            return;
        }
        if(target.getRole() !="Merchand") {
            if(target.getCoinsCount() <1){
                game.nextTurn();
                return;
            }
            target.removeCoins(1);
            this->addCoins(1);
            game.nextTurn();
        }
        else{
            target.removeCoins(2);
            game.nextTurn();
        }
        
    }
    void PlayerTest::sanction(PlayerTest& target) {
        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("It's not your turn");
        }
        if(this->getCoinsCount() < 3 ) {
            throw std::runtime_error("Not enough coins to sanction");
        }
        lastMove = "sanction";
        removeCoins(3);
        if (target.getRole() == "Judge") {
            removeCoins(1);
        }
        lastTarget = target.getName();
        target.markSanctioned(true);
        game.nextTurn();
    }
    void PlayerTest::coup(PlayerTest& target) {
        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("It's not your turn");
        }
        if (coins < 7) {
            //std::cout << "Player " << this->getName() << " tried to coup " << target.getName() << " but doesn't have enough coins." << std::endl;
            throw std::runtime_error("Not enough coins to coup");
        }
        removeCoins(7);
        lastMove = "coup";
        lastTarget = target.getName();
        bool isSaved = false;
        for(auto& player : game.getPlayers()) {
            if (player->getRole() == "General") {
                auto general = dynamic_cast<General*>(player);
                if (general == nullptr) {
                    throw std::runtime_error("Player is not a General or the dinamic_cast failed");
                }
                auto Isfound = general->getList().find(target.getName());
                if (Isfound != general->getList().end() && Isfound->second == true) {
                    isSaved = true;
                    break;
                }
            }
        }
        if (isSaved) {
            std::cout << target.getName() << " was saved from coup by a General" << std::endl;
            game.nextTurn();
            return;
        }
        game.removePlayer(&target);
        game.nextTurn();
    }
        
}       
