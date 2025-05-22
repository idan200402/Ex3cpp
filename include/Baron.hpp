#pragma once
#include "Player.hpp"

namespace ex3 {
    class Baron : public Player {
    public:
        Baron(Game& game, const std::string& name);
        void invest();
        void whenSanctioned(const std::string& actionName) override;
    };
}