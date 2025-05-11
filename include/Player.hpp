// idan.shumski@gmail.com

#pragma once
#include <string>

namespace ex3 {
    class Game;

    class Player {
    protected:
        Game& game;
        std::string name;
        std::string role;
        int coins;
        bool alive;
        std::string lastMove;
        std::string lastTarget;
        bool sanctioned;

    public:
        Player(Game& game, const std::string& name);
        virtual ~Player() = default;

        const std::string& getName() const;
        const std::string& getRole() const;
        int getCoinsCount() const;
        bool isAlive() const;
        const std::string& getLastMove() const;
        const std::string& getLastTarget() const;
        bool isSanctioned() const;

        virtual void gather() = 0;
        virtual void tax() = 0;
        virtual void bribe() = 0;
        virtual void arrest(Player& target) = 0;
        virtual void sanction(Player& target) = 0;
        virtual void coup(Player& target) = 0;
        virtual void undo(Player& target) = 0;

        void addCoins(int amount);
        void removeCoins(int amount);
        void deactivate();
        void markSanctioned(bool status);
    };
}
