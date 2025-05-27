#include "../include/Spy.hpp"
#include "../include/Game.hpp"
#include <iostream>
#include <stdexcept>

namespace ex3 {
    // Constructor for Spy class
    //initializes the Spy with a name and associates it with a game instance, also with his role.
    Spy::Spy(Game& game, const std::string& name)
        : Player(game, name) {
        this->role = "Spy";
        this->arrestdisabled = {};
        this->indexOfArrestDisabled = {};
        this->blockArrestCount = 0;
    }
    void Spy::inspectCoins(Player& target) {
        // If it's not the player's turn, throw an error.
        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("It's not your turn");
        }
        // printing the target's coins count , it simbolizes the Spy inspecting the target's coins.
        lastMove = "inspect";
        lastTarget = target.getName();
        std::cout << "Spy " << this->getName() << " inspected " << target.getName() << "'s coins: " << target.getCoinsCount() << std::endl;
    }
    void Spy::blockArrest(Player& target) {
        //You can block an arrest only once per turn, so if the blockArrestCount is 1, throw an error.
        if (blockArrestCount == 1) {
            game.nextTurn();
            throw std::runtime_error("You have allready blocked in this turn");
        }
        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("It's not your turn");
        }
        //Update the indexOfArrestDisabled map with the current turn counter.
        lastMove = "blockArrest";
        lastTarget = target.getName();
        arrestdisabled[target.getName()] = true;
        blockArrestCount++;
    }
    std::unordered_map<std::string, bool>& Spy::getArrestDisabled() {
        return arrestdisabled;
    }
    std::unordered_map<std::string, bool>& Spy::getList() {
        return getArrestDisabled();
    }
    void Spy::onStartTurn() {
        //iterate through the arrestdisabled map and remove players whose arrest block has expired.
        int currentTurn = game.getTurnCounter();
        for(auto it = arrestdisabled.begin(); it != arrestdisabled.end();) {
            const std::string& playerName = it->first;
            if(indexOfArrestDisabled.count(playerName)>0 && indexOfArrestDisabled[playerName] < currentTurn) {
                it = arrestdisabled.erase(it);
                indexOfArrestDisabled.erase(playerName);
            } else {
                ++it;
            } 
        }
        //then call the Player's onStartTurn method to handle any other start of turn logic.
        Player::onStartTurn();
    }
    void Spy::mainAbility(Player* target) {
        // Check if the target is null before proceeding
        if (!target) {
            throw std::runtime_error("Target cannot be null");
        }
        // Call the private method inspectCoins to perform the Spy's main ability
        inspectCoins(*target);
    }
    void Spy::secondaryAbility(Player* target) {
        // Check if the target is null before proceeding
         
        if (!target) {
            throw std::runtime_error("Target cannot be null");
        }
    // Call the private method blockArrest to perform the Spy's secondary ability
        blockArrest(*target);
    }
        
}