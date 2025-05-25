//idan.shumski@gmail.com
#pragma once
#include <string>
#include <vector>

namespace ex3 {
    class PlayerTest;
    class Game {
    private:
        bool shuttingDown;
        int turnCounter;
        int currentPlayerIndex;
        std::vector<PlayerTest*> players;
    public:
        Game();
        void addPlayer(PlayerTest* player);
        std::vector<PlayerTest*> getPlayers() const;
        std::string turn() const;
        std::string getWinner() const;
        void nextTurn();
        bool isPlayerTurn(PlayerTest* p) const;
        void removePlayer(PlayerTest* p);
        int getActivePlayerCount() const;
        int getTurnCounter() const;
        ~Game();
        bool isShuttingDown() const;
        bool isPlayerAlive(PlayerTest& p) const;
        void forceTurnTo(PlayerTest* player);
      
    };  
}
        