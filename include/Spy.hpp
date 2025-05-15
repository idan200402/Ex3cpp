#pragma once 
#include "Player.hpp"
#include <unordered_map>
 namespace ex3 {
    class Spy : public Player {
    private:
        int blockArrestCount;
        std::unordered_map<std::string, bool> arrestdisabled;
    public:
        Spy(Game& game, const std::string& name);
        int inspectCoins(Player& target);
        void blockArrest(Player& target); 
        std::unordered_map<std::string, bool>& getArrestDisabled() {
            return arrestdisabled;
        }
    };
}
        