// idan.shumski@gmail.com

#include <iostream>
#include "../include/Game.hpp"
#include "../include/RoleFactory.hpp"
#include <stdexcept>

using namespace ex3;

int main() {
    try{
        ex3::Game game;
        Player* p1 = RoleFactory::createRole(game, "Barak", "Baron");
        Player* p2 = RoleFactory::createRole(game, "Rod", "Baron");
        auto players = game.getPlayers();
        std::cout << "Players in the game:" << std::endl;
        for (const auto& player : players) {
            std::cout << player->getName() << " - " << player->getRole() << std::endl;
        }


    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    
    return 0;
}
