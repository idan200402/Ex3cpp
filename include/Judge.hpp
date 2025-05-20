#pragma once
#include "Player.hpp"
#include <unordered_map>

namespace ex3 {

    class Judge : public Player {
        private:
            std::unordered_map<std::string, bool> blockBribed;
            std::unordered_map<std::string, int> indexOfBlockBribed;
        public:
            Judge(Game& game, const std::string& name);
            void blockBribe(Player& target);
            std::unordered_map<std::string, bool>& getBlockBribed();
            void onStartTurn() override;
    };
}