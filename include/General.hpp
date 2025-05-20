//idan.shumski@gmail.com
#pragma once
#include "Player.hpp"
#include <unordered_map>

namespace ex3 {

    class General : public Player {
        private:
            std::unordered_map<std::string, bool> savedFromCoup;
            std::unordered_map<std::string, int> indexOfSavedFromCoup;
        public:
            General(Game& game, const std::string& name);
            void saveFromCoup(Player& target);
            std::unordered_map<std::string, bool>& getSavedFromCoup();
            void onStartTurn() override;
    };
}
            