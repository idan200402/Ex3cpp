//idan.shumski@gmail.com
#pragma once
#include <string>
#include <vector>

namespace ex3 {
    class Player;
    class Game {
    private:
        bool shuttingDown;
        int turnCounter;
        int currentPlayerIndex;
        std::vector<Player*> players;
    public:
        Game();
        void addPlayer(Player* player);
        std::vector<Player*> getPlayers() const;
        std::string turn() const;
        std::string getWinner() const;
        void nextTurn();
        bool isPlayerTurn(Player* p) const;
        void removePlayer(Player* p);
        int getActivePlayerCount() const;
        int getTurnCounter() const;
        ~Game();
        bool isShuttingDown() const;
        bool isPlayerAlive(Player& p) const;
        void forceTurnTo(Player* player);
      
    };  
}
        