// idan.shumski@gmail.com

#pragma once
#include "Player.hpp"

namespace ex3 {

    class Governor : public Player {
    public:
        Governor(Game& game, const std::string& name);

        void gather() override;
        void tax() override;
        void bribe() override;
        void arrest(Player& target) override;
        void sanction(Player& target) override;
        void coup(Player& target) override;
        void undo(Player& target) override;
    };

}
