#pragma once 
#include "Player.hpp"
#include <unordered_map>
 namespace ex3 {
    class Spy : public Player {
    private:
        std::unordered_map<std::string, bool> arrestdisabled;
    public:
        Spy(Game& game, const std::string& name);
        void lookAndEnableArrest(Player& target);
        const std::unordered_map<std::string, bool>& getArrestDisabled() const;
    };
}
        