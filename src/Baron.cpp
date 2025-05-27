#include "../include/Baron.hpp"
#include "../include/Game.hpp"
#include <stdexcept>

namespace ex3 {
    //constructor for Baron class
    Baron::Baron(Game& game, const std::string& name)
        : Player(game, name) {
        this->role = "Baron";
    }

    void Baron::invest() {
        // if its not the player's turn or if the player does not have enough coins, throw an error.
        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("It's not your turn");
        }
        if(this->coins < 3) {
            throw std::runtime_error("Not enough coins to invest");
        }
        // remove 3 coins from the player and add 6 coins to the player.
        addCoins(3);
        lastMove = "invest";
        lastTarget = "";
        game.nextTurn();
    }
    void Baron::whenSanctioned(const std::string& actionName) {
        // If the Baron is sanctioned while performing a financial action, he receives 1 coin.
        if (actionName== "gather" || actionName == "tax") {
            addCoins(1);
        }
    }
    // The Baron's main ability is to invest coins, which does not require a target.
    void Baron::mainAbility(Player* target) {
        (void)target; // To avoid unused parameter warning
        invest();
    }
    void Baron::secondaryAbility(Player* /*target*/){
        // The Baron does not have a secondary ability defined in this context.
        throw std::runtime_error("Baron does not have a secondary ability");
    }
}
        
          
            