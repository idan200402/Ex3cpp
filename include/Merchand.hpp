#pragma once
#include "Player.hpp"
 
namespace ex3 {
    class Merchand : public Player{
        public:
            Merchand(Game& game, const std::string& name);
            void onStartTurn() override;
    };
}