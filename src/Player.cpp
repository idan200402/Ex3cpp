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
    // Constructor for Player class
    Player::Player(Game& game, const std::string& name)
        : game(game),
          name(name),
          // Initialize role to an empty string
          role(""),
            // Initialize coins to 0
          coins(0),
            // Initialize alive status to true
          alive(true),
          // Initialize lastMove and lastTarget to empty strings
          lastMove(""),
          lastTarget(""),
            // Initialize sanctioned status to false
          sanctioned(false),
            // Initialize hasNextTurn to false
          hasNextTurn(false)
    {
        // Check if the name is empty and throw an exception if it is
        if (name.empty()) {
            throw std::invalid_argument("Name cannot be empty");
        }
        //std::cout << "Created player: " << name << " with coins = " << coins << std::endl;
        game.addPlayer(this);
    }
    // Getter methods for Player class
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
        // Check if the amount is negative and throw an exception if it is
        if (amount < 0) {
            throw std::invalid_argument("Amount must be non-negative");
        }
        coins += amount;
    }

    void Player::removeCoins(int amount) {
        // Check if the amount is negative or if there are not enough coins and throw an exception if either condition is met
        if (amount < 0) {
            throw std::invalid_argument("Amount must be non-negative");
        }
        if (coins < amount) {
            throw std::runtime_error("Not enough coins");
        }
        coins -= amount;
    }
    void Player::onStartTurn() {
        // Check if the game is shutting down, if so, throw an error
        if(game.isShuttingDown()) {
            throw std::runtime_error("Game is shutting down");
        }
        //std::cout << this->getName() << " is starting their turn with " << this->getCoinsCount() << " coins." << std::endl;
        //if the has more than or equal to 10 coins, it will the first player that is not him and is alive.
        if(coins >= 10) {
            for(auto& player : game.getPlayers()) {
                if (player!= this &&player->isAlive()) {
                    this->coup(*player);
                    break;
                }
            }
        }
    }
    // Destructor for Player class
    Player::~Player()  = default;

    void Player::deactivate() {
        // Set the player's alive status to false
        alive = false;
    }

    void Player::markSanctioned(bool status) {
        //the status parameter is used to set the sanctioned status of the player , usually it is true when the player is sanctioned.
        sanctioned = status;
    }
    void Player::gather() {
        // Check if it's the player's turn and if they are not sanctioned
        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("It's not your turn");
        }
        // If the player is sanctioned, call the whenSanctioned method and throw an error
        if (isSanctioned()) {
            whenSanctioned("gather");
            game.nextTurn();
            throw std::runtime_error("You are sanctioned");
        }
        // Add 1 coin to the player's coins count
        addCoins(1);
        lastMove = "gather";
        lastTarget = "";
        game.nextTurn();
    }
    void Player::tax() {
        // Check if it's the player's turn and if they are not sanctioned
        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("It's not your turn");
        }
        bool isEnabled = false;
        //iterate through all players to check if at least one Governor has enabled tax on this player.
        for(auto& player : game.getPlayers()) {
            if (player->getRole() == "Governor") {
                auto governor = dynamic_cast<Governor*>(player);
                // Check if the dynamic_cast failed
                if (governor == nullptr) {
                    throw std::runtime_error("Player is not a Governor or the dinamic_cast failed");
                }
                // Check if the Governor has enabled tax for this player
                auto Isfound = governor->getList().find(this->getName());
                if (Isfound != governor->getList().end() && Isfound->second == true) {
                    Isfound->second = false;
                    // If at least one Governor has enabled tax, set isEnabled to true
                    isEnabled = true;
                }
            }
        }
        // If the player blocked by a Governor , the turn is over.
        if (isEnabled) {
            game.nextTurn();
            //whenSanctioned("tax");
            throw std::runtime_error("At least one Governor has enabled tax");
        }   
        //Adding 2 coins to the player.
        addCoins(2);
        lastMove = "tax";
        lastTarget = "";
        game.nextTurn();
    }
   void Player::bribe() {
    // Check if it's the player's turn, if not, throw an error
    if (!game.isPlayerTurn(this)) {
        throw std::runtime_error("It's not your turn");
    }
    // Check if the players last move was a bribe, if so, throw an error
    if (lastMove == "bribe") {
        throw std::runtime_error("You cannot bribe twice in a row");
    }
    // check if the player has enough coins to bribe, if not, throw an error
    if (coins < 4) {
        throw std::runtime_error("Not enough coins to bribe");
    }
    removeCoins(4);

    bool isBlocked = false;
    // Iterate through all players to check if at least one Judge has blocked the bribe
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
    // If the bribe was not blocked, set hasNextTurn to true, otherwise throw an error
    if (!isBlocked) {
        hasNextTurn = true;
    } else {
       throw std::runtime_error("Bribe was blocked by a Judge");
    }

    
}


void Player::arrest(Player& target) {
    // Check if it's the player's turn and if the target is alive
    if (!game.isPlayerTurn(this)) {
        throw std::runtime_error("It's not your turn");
    }
    //check if the player is alive, if not, throw an error
    if(game.isPlayerAlive(target) == false) {
        throw std::runtime_error("Target is not alive");
    }
    
    bool isBlocked = false;
    // Iterate through all players to check if at least one Spy has blocked the arrest
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
    // If the arrest is blocked by a Spy , the turn is over.
    if (isBlocked) {
        game.nextTurn();
        std::cout << target.getName() << " was blocked from being arrested by a Spy" << std::endl;
    }
    // Check if the last move was an arrest and if the target is the same as the last target, if so, throw an error
    if(lastMove == "arrest" && lastTarget == target.getName()) {
        throw std::runtime_error("You cannot arrest the same player twice in a row");
    }
    // Check if the target is a General, if so, add 1 coin to this player and end the turn
    lastMove = "arrest";
    lastTarget = target.getName();
    if(target.getRole() == "General") {
        addCoins(1);
        game.nextTurn();
        return;
    }
    // If the target is not a merchand, remove 1 coin from the target and add 1 coin to this player, otherwise remove 2 coins from the target
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
    void Player::sanction(Player& target) {
        // Check if it's the player's turn and if they have enough coins to sanction
        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("It's not your turn");
        }
        if(this->getCoinsCount() < 3 ) {
            throw std::runtime_error("Not enough coins to sanction");
        }
        //updating the attributes of the player.
        lastMove = "sanction";
        removeCoins(3);
        // If the target is a judge, remove 1 coin from this player
        if (target.getRole() == "Judge") {
            removeCoins(1);
        }
        lastTarget = target.getName();
        target.markSanctioned(true);
        game.nextTurn();
    }
    void Player::coup(Player& target) {
        // Check if it's the player's turn and if they have enough coins to coup
        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("It's not your turn");
        }
        // Check if the target has enough coins to coup, if not, throw an error
        if (coins < 7) {
            //std::cout << "Player " << this->getName() << " tried to coup " << target.getName() << " but doesn't have enough coins." << std::endl;
            throw std::runtime_error("Not enough coins to coup");
        }
        removeCoins(7);
        lastMove = "coup";
        lastTarget = target.getName();
        bool isSaved = false;
        // Iterate through all players to check if at least one General has saved the target from coup
        for(auto& player : game.getPlayers()) {
            if (player->getRole() == "General") {
                auto general = dynamic_cast<General*>(player);
                // Check if the dynamic_cast failed
                if (general == nullptr) {
                    throw std::runtime_error("Player is not a General or the dinamic_cast failed");
                }
                // Check if the General has saved the target from coup
                auto Isfound = general->getList().find(target.getName());
                if (Isfound != general->getList().end() && Isfound->second == true) {
                    isSaved = true;
                    break;
                }
            }
        }
        // If the target was saved from coup, this player lost 5 coins and the target is not removed from the game
        if (isSaved) {
            std::cout << target.getName() << " was saved from coup by a General" << std::endl;
            game.nextTurn();
            return;
        }
        // If the target was not saved from coup, remove the target from the game
        game.removePlayer(&target);
        game.nextTurn();
    }
        
}       
