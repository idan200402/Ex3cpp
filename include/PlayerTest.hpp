// idan.shumski@gmail.com

#pragma once
#include <string>
#include "../include/Game.hpp"
class Governor;
namespace ex3 {
    

    class PlayerTest {
    protected:
        Game& game;
        std::string name;
        std::string role;
        int coins;
        bool alive;
        std::string lastMove;
        std::string lastTarget;
        bool sanctioned;
        virtual void whenSanctioned(const std::string& /*actionName*/) {}
        
    public:
        virtual void onStartTurn();
        bool hasNextTurn;
        PlayerTest(Game& game, const std::string& name);

        const std::string& getName() const;
        const std::string& getRole() const;
        int getCoinsCount() const;
        bool isAlive() const;
        const std::string& getLastMove() const;
        const std::string& getLastTarget() const;
        bool isSanctioned() ;

        virtual void gather();
        virtual void tax();
        virtual void bribe();
        virtual void arrest(PlayerTest& target);
        virtual void sanction(PlayerTest& target);
        virtual void coup(PlayerTest& target);

        void addCoins(int amount);
        void removeCoins(int amount);
        void deactivate();
        void markSanctioned(bool status);
        virtual ~PlayerTest() ;
    };
}
