// idan.shumski@gmail.com

#pragma once
#include "Player.hpp"
#include <unordered_map>

namespace ex3 {

    class Governor : public Player {
    private:
        std::unordered_map<std::string, bool> enabledTax;
        std::unordered_map<std::string, int> indexOfEnabledTax;
    public:
        Governor(Game& game, const std::string& name);
        void undo(Player& target);
        std::unordered_map<std::string, bool>& getEnabledTax();
        void tax() override;
        void onStartTurn() override;
    };

}
