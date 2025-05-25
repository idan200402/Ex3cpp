//idan.shumski@gmail.com
#pragma once
#include "Player.hpp"
#include <unordered_map>

namespace ex3 {

    class General : public Player {
        private:
            std::unordered_map<std::string, bool> savedFromCoup;
            std::unordered_map<std::string, int> indexOfSavedFromCoup;
            std::unordered_map<std::string, bool>& getSavedFromCoup();
            void saveFromCoup(PlayerTest& target);
        public:
            General(Game& game, const std::string& name);
            void mainAbility(Player* target) override;
            void secondaryAbility(Player* target) override; 
            void onStartTurn() override;
            std::unordered_map<std::string, bool>& getList() override;
    };
}
            