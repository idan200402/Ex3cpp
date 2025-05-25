#include "../include/Baron.hpp"
#include "../include/Game.hpp"
#include <stdexcept>

namespace ex3 {
    Baron::Baron(Game& game, const std::string& name)
        : Player(game, name) {
        this->role = "Baron";
    }

    void Baron::invest() {
        if (!game.isPlayerTurn(this)) {
            throw std::runtime_error("It's not your turn");
        }
        if(this->coins < 3) {
            throw std::runtime_error("Not enough coins to invest");
        }
        addCoins(3);
        lastMove = "invest";
        lastTarget = "";
        game.nextTurn();
    }
    void Baron::whenSanctioned(const std::string& actionName) {
        if (actionName== "gather" || actionName == "tax") {
            addCoins(1);
        }
    }
    void Baron::mainAbility(Player* target) {
        (void)target; // To avoid unused parameter warning
        invest();
    }
    void Baron::secondaryAbility(Player* /*target*/){
        // The Baron does not have a secondary ability defined in this context.
        throw std::runtime_error("Baron does not have a secondary ability");
    }
}
        
          
            