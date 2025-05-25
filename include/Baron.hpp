#pragma once
#include "Player.hpp"

namespace ex3 {
    class Baron : public Player {
    private:
        void invest();
    public:
        Baron(Game& game, const std::string& name);
        void whenSanctioned(const std::string& actionName) override;
        void mainAbility(Player* target) override;
        void secondaryAbility(Player* target) override;

    };
}