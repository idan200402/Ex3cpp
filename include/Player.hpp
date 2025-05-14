// idan.shumski@gmail.com

#pragma once
#include <string>
class Game;
class Governor;
namespace ex3 {
    

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

        virtual void gather();
        virtual void tax();
        virtual void bribe();
        virtual void arrest(Player& target);
        virtual void sanction(Player& target);
        virtual void coup(Player& target);

        void addCoins(int amount);
        void removeCoins(int amount);
        void deactivate();
        void markSanctioned(bool status);
    };
}
