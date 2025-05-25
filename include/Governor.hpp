// idan.shumski@gmail.com

#pragma once
#include "Player.hpp"
#include <unordered_map>

namespace ex3 {

    class Governor : public Player {
    private:
        std::unordered_map<std::string, bool> enabledTax;
        std::unordered_map<std::string, int> indexOfEnabledTax;
         std::unordered_map<std::string, bool>& getEnabledTax();
         void undo(PlayerTest& target);
    public:
        Governor(Game& game, const std::string& name);
        void tax() override;
        void onStartTurn() override;
        void mainAbility(Player* target) override;
        void secondaryAbility(Player* target) override;
        std::unordered_map<std::string, bool>& getList() override; 
    };

}
