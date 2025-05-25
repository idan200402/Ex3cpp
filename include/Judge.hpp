#pragma once
#include "Player.hpp"
#include <unordered_map>

namespace ex3 {

    class Judge : public Player {
        private:
            std::unordered_map<std::string, bool> blockBribed;
            std::unordered_map<std::string, int> indexOfBlockBribed;
            void blockBribe(Player& target);
             std::unordered_map<std::string, bool>& getBlockBribed();
        public:
            Judge(Game& game, const std::string& name);
            void onStartTurn() override;
            void mainAbility(Player* target) override;
            void secondaryAbility(Player* target) override;
            std::unordered_map<std::string, bool>& getList() override;
    };
}