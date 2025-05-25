#pragma once 
#include "Player.hpp"
#include <unordered_map>
 namespace ex3 {
    class Spy : public Player {
    private: 
        int blockArrestCount;
        std::unordered_map<std::string, bool> arrestdisabled;
        std::unordered_map<std::string, int> indexOfArrestDisabled;
        void inspectCoins(Player& target);
        void blockArrest(Player& target); 
        std::unordered_map<std::string, bool>& getArrestDisabled();
    public:
        int inspectedCoins;
        Spy(Game& game, const std::string& name);
        void onStartTurn() override;
        void mainAbility(Player* target) override;
        void secondaryAbility(Player* target) override;
        std::unordered_map<std::string, bool>& getList() override;
    };
}
        