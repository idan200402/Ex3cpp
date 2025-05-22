#pragma once 
#include "Player.hpp"
#include <unordered_map>
 namespace ex3 {
    class Spy : public Player {
    private:
        int blockArrestCount;
        std::unordered_map<std::string, bool> arrestdisabled;
        std::unordered_map<std::string, int> indexOfArrestDisabled;
    public:
        Spy(Game& game, const std::string& name);
        int inspectCoins(Player& target);
        void blockArrest(Player& target); 
        std::unordered_map<std::string, bool>& getArrestDisabled();
        void onStartTurn() override;
    };
}
        