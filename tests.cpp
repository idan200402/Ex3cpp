#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "include/Game.hpp"
#include "include/Player.hpp"
#include "include/PlayerTest.hpp"
#include "include/RoleFactory.hpp"
#include "include/Governor.hpp"
#include "include/Baron.hpp"
#include "include/General.hpp"
#include "include/Judge.hpp"
#include "include/Merchand.hpp"
#include "include/Spy.hpp"
#include <stdexcept>
#include <iostream>
#include <sstream>
using namespace ex3;
//Testing the Game class and its methods
TEST_CASE("Testing Game Class Functionality") {
    ex3::Game game;
    SUBCASE("Adding Players") {
        //testing the addPlayer method its happening when we initialize the player.
        new PlayerTest(game, "Barak");
        new PlayerTest(game, "Rod");
        auto players = game.getPlayers();
        CHECK(players.size() == 2);
        CHECK(players[0]->getName() == "Barak");
        CHECK(players[1]->getName() == "Rod");
        //because the name is the identifier of the player, we should not be able to add a player with the same name.
        CHECK_THROWS( new PlayerTest(game, "Barak")); 
        //trying to add seven players to the game , the seventh player should throw an error.
        new PlayerTest(game, "C");
        new PlayerTest(game, "D");
        new PlayerTest(game, "E");
        new PlayerTest(game, "F");
        CHECK_THROWS(new PlayerTest(game, "G"));
    }
    SUBCASE("Removing Players") {
        Player* player1 = new PlayerTest(game, "Barak");
        Player* player2 = new PlayerTest(game, "Rod");
        game.removePlayer(player1);
        //the number should be 1 because we removed one player
        auto players = game.getPlayers();
        CHECK(players.size() == 1);
        CHECK(players[0]->getName() == "Rod");
        game.removePlayer(player2);
        players = game.getPlayers();
        CHECK(players.size() == 0);
        //the removed players should be dead , but still is address is in the array.
        CHECK(player1->isAlive() == false);
        CHECK(player2->isAlive() == false);
    }
    //testing the method that returns the current player's name turn.
    SUBCASE("Game Turn Functionality") {
        //should throw an error if there are no players in the game
        CHECK_THROWS(game.turn());
        Player* player1 = new PlayerTest(game, "Barak");
        Player* player2 = new PlayerTest(game, "Rod");
        //the first player to be added to the game is the first player to play
        CHECK(game.turn() == "Barak");
        player1->gather();
        CHECK(game.turn() == "Rod");
    }
    SUBCASE("Game Winner Functionality") {
        //there are no players in the game
        CHECK_THROWS(game.getWinner());
        Player* player1 = new PlayerTest(game, "Barak");
        new PlayerTest(game, "Rod");
        //the game is still ongoing
        CHECK_THROWS(game.getWinner());
        game.removePlayer(player1);
        //there is only one player alive in the game . thus he is the winner.
        CHECK(game.getWinner() == "Rod");
    }
    SUBCASE("Game Turn Counter Functionality") {
        Player* player1 = new PlayerTest(game, "Barak");
        Player* player2 = new PlayerTest(game, "Rod");
        CHECK(game.getTurnCounter() == 0);
        player1->gather();
        CHECK(game.getTurnCounter() == 1);
        player2->gather();
        CHECK(game.getTurnCounter() == 2);
    }
    SUBCASE("Game Active Player Count Functionality") {
        Player* player1 = new PlayerTest(game, "Barak");
        Player* player2 = new PlayerTest(game, "Rod");
        CHECK(game.getActivePlayerCount() == 2);
        game.removePlayer(player1);
        CHECK(game.getActivePlayerCount() == 1);
        game.removePlayer(player2);
        CHECK(game.getActivePlayerCount() == 0);
    }
}
TEST_CASE("Testing Player Class Functionality") {
    ex3::Game game;
    SUBCASE("Player Initialization") {
        Player* player1 = new PlayerTest(game, "Barak");
        CHECK(player1->getName() == "Barak");
        CHECK(player1->getCoinsCount() == 0);
        CHECK(player1->isAlive() == true);
        CHECK(player1->getLastMove() == "");
        CHECK(player1->getLastTarget() == "");
        CHECK(player1->isSanctioned() == false);
        //if a player has no name it should throw an error
        CHECK_THROWS(new PlayerTest(game, ""));
    }
    SUBCASE("Add and subtract coins") {
        Player* player1 = new PlayerTest(game, "Barak");
        player1->addCoins(5);
        CHECK(player1->getCoinsCount() == 5);
        player1->removeCoins(2);
        CHECK(player1->getCoinsCount() == 3);
        //if we try to remove more coins than we have it should throw an error
        CHECK_THROWS(player1->removeCoins(4));
        //we can not add or remove negative coin number
        CHECK_THROWS(player1->removeCoins(-1));
        CHECK_THROWS(player1->addCoins(-1));
    }
    SUBCASE("Player Actions") {
        SUBCASE("Gather") {
            Player* player1 = new PlayerTest(game, "Barak");
            new PlayerTest(game, "Rod");
            player1->gather();
            CHECK(player1->getCoinsCount() == 1);
            CHECK(player1->getLastMove() == "gather");
            CHECK(player1->getLastTarget() == "");
            //since its not his turn he should not be able to gather
            CHECK_THROWS(player1->gather());
        }
        SUBCASE("Tax") {
            Player* player1 = new PlayerTest(game, "Barak");
            new PlayerTest(game, "Rod");
            player1->tax();
            CHECK(player1->getCoinsCount() == 2);
            CHECK(player1->getLastMove() == "tax");
            CHECK(player1->getLastTarget() == "");
            //since its not his turn he should not be able to tax
            CHECK_THROWS(player1->tax());
        }
        SUBCASE("Bribe") {
            Player* player1 = new PlayerTest(game, "Barak");
            new PlayerTest(game, "Rod");
            //he has to have at least 4 coins to bribe
            CHECK_THROWS(player1->bribe());
            player1->addCoins(4);
            player1->bribe();
            //now he should have 2 turns in a row on top of the bribe itself.
            CHECK(player1->getCoinsCount() == 0);
            CHECK(player1->getLastMove() == "bribe");
            CHECK(player1->getLastTarget() == "");
            CHECK(game.turn() == "Barak");
            //if he tries to bribe again it should throw an error , player cant bribe twice in a row
            CHECK_THROWS(player1->bribe());
            //he has to action after the bribe
            CHECK_NOTHROW(player1->gather());
            CHECK_NOTHROW(player1->gather());
        }
        SUBCASE("Arrest") {
            Player* player1 = new PlayerTest(game, "Barak");
            Player* player2 = new PlayerTest(game, "Rod");
            player1->gather();
            player2->arrest(*player1);
            CHECK(player1->getCoinsCount() == 0);
            CHECK(player2->getCoinsCount() == 1);
            player1->gather();
            player2->gather();//stimulate another action to not arrest twice.
            player1->gather();
            player1->removeCoins(2); // setting his coins count to 0.
            int delta0coins = player1->getCoinsCount();
            player2->arrest(*player1);     
            int delta1coins = player1->getCoinsCount();
            //since the player that was arrested had 0 coins, the arrest dont have coins to take.
            CHECK(delta0coins == delta1coins);      
            player1->gather(); 
            //he should not be able to arrest the same player twice in a row.
            CHECK_THROWS(player2->arrest(*player1));
            game.removePlayer(player2);
            //if the player is dead he should not be able to arrest him.
            CHECK_THROWS(player1->arrest(*player2));
        }
        SUBCASE("Sanction") {
            Player* player1 = new PlayerTest(game, "Barak");
            Player* player2 = new PlayerTest(game, "Rod");
            player1->addCoins(3);
            player1->sanction(*player2);
            CHECK(player1->getLastMove() == "sanction");
            CHECK(player1->getLastTarget() == "Rod");
            //Barak is the one who sanctioned Rod, so Rod won't be able to use gather and tax.
            CHECK_THROWS(player2->gather());
            //then player 2 decides to make a valid action.
            CHECK_THROWS(player2->arrest(*player1));
            player1->gather();
            //he should be able to use gather in the next turn iteration.
            CHECK_NOTHROW(player2->gather());
        }
        SUBCASE("Coup") {
            Player* player1 = new PlayerTest(game, "Barak");
            Player* player2 = new PlayerTest(game, "Rod");
            player1->addCoins(7);
            player1->coup(*player2);
            CHECK(player1->getLastMove() == "coup");
            CHECK(player1->getLastTarget() == "Rod");
            CHECK(player2->isAlive() == false);
        }

    }


}
TEST_CASE("Testing RoleFactory Class Functionality") {
    ex3::Game game;
    SUBCASE("Creating Players with Roles") {
        Player* player1 = RoleFactory::createRole(game, "Barak", "Baron");
        Player* player2 = RoleFactory::createRole(game, "Rod", "Judge");
        CHECK(player1->getName() == "Barak");
        CHECK(player1->getRole() == "Baron");
        CHECK(player2->getName() == "Rod");
        CHECK(player2->getRole() == "Judge");
        //if we try to create a player with the same name it should throw an error
        CHECK_THROWS(RoleFactory::createRole(game, "Barak", "Governor"));
        //if we try to create a player with an invalid role it should throw an error
        CHECK_THROWS(RoleFactory::createRole(game, "C", "InvalidRole"));
        //if we try to create a player with an empty name it should throw an error
        CHECK_THROWS(RoleFactory::createRole(game, "", "Baron"));
    }
}
TEST_CASE("Testing The Special Abilities of the different Roles") {
    ex3::Game game;
    SUBCASE("Governor Actions") {
        ex3::Governor* governor = new ex3::Governor(game, "Barak");
        Player* player = new Spy(game, "Rod");
        governor->tax();
        CHECK(governor->getCoinsCount() == 3); //he receives 3 coins instead of 2
        player->gather();
        governor->mainAbility(player); //the governor enables tax on the player
        CHECK(governor->getList().size() == 1);
        CHECK_THROWS(player->tax()); // the governor enabled tax on the player
    }
    SUBCASE("Spy Actions") {
        Spy* spy = new Spy(game, "Barak");
        Player* player = new Judge(game, "Rod");
        player->addCoins(5);
        std::stringstream buffer;
        std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());
        spy->mainAbility(player); // should print the number inspected
        std::cout.rdbuf(oldCout); // restore cout
        std::string output = buffer.str();
        CHECK(output.find("coins: 5") != std::string::npos); // check if the output contains the expected string
        CHECK(spy->getLastMove() == "inspect");
        CHECK(spy->getLastTarget() == "Rod");
        CHECK(spy->getList().size() == 0);
        //the turn is still not over, the spy can block the arrest
        spy->secondaryAbility(player);
        CHECK(spy->getList().size() == 1);
        CHECK_THROWS(player->arrest(*spy)); // the player should not be able to arrest the spy since the spy blocked the arrest
    }
    SUBCASE("Baron Actions") {
        Baron* baron = new Baron(game, "Barak");
        Player* player = new General(game, "Rod");
        CHECK_THROWS(baron->mainAbility(nullptr)); // should throw an error since the player has no coins
        player->addCoins(3); // the player has 3 coins to be able to sanction the baron
        baron->addCoins(3);
        CHECK_NOTHROW(baron->mainAbility(nullptr)); // should not throw an error since the player has coins
        CHECK(baron->getCoinsCount() == 6); // should return the number of coins after the investment
        CHECK(baron->getLastMove() == "invest");
        CHECK(baron->getLastTarget() == "");
        //if the baron is sanctioned he should get 1 coin
        player->sanction(*baron);
        CHECK_THROWS(baron->gather());//should throw an error since the player is sanctioned , but he still gets 1 coin
        CHECK(baron->getCoinsCount() == 7); // should return the number of coins after the investment
        CHECK(baron->getLastTarget() == "");
    }
    SUBCASE("General Actions") {
        SUBCASE("A: testing the General's ability to block a coup") {
            General* general = new General(game, "Barak");
            Player* player1 = new Baron(game, "Rod");
            Player* player2 = new Baron(game, "Lechner");
            CHECK_THROWS(general->mainAbility(player2)); // General has no coins to save Lechner.
            general->addCoins(5); // General has 5 coins to block the coup of Lechner.
            player1->addCoins(7); // Rod has 7 coins to coup Lechner.
            CHECK_NOTHROW(general->mainAbility(player2)); // General saves Lechner from being couped.
            CHECK(general->getList().size() == 1); // General has saved Lechner from coup.
            CHECK(general->getCoinsCount() == 0); // General has spent 5 coins to save Lechner.
            //He has another turn to play.
            CHECK_NOTHROW(general->gather());
            player1->coup(*player2); // Rod tries to coup Lechner.
            CHECK(player2->isAlive() == true); // Lechner is still alive because of the General's action.
            CHECK_NOTHROW(player2->gather()); // Lechner can gather coins.
        }
        SUBCASE("B: testing the General's ability to recieve a coin after he being arrested") {
            Player* player1 = new ex3::Governor(game, "Barak");
            General* general = new General(game, "Rod");
            general->addCoins(5); //he need to have coins to be arrested
            player1->arrest(*general); // Barak arrests Rod.
            CHECK(player1->getCoinsCount() == 1); // Barak should have 1 coin after arresting Rod.
            CHECK(general->getCoinsCount() == 5); // Rod should have 5 coins after being arrested.
        }
    }
    SUBCASE("Judge Actions") {
        SUBCASE("A: testing the Judge's ability to cancel a bribe") {
            Judge* judge = new Judge(game, "Barak");
            Player* player1 = new Baron(game, "Rod");
            player1->addCoins(4); // Rod has 4 coins to bribe.
            CHECK_NOTHROW(judge->mainAbility(player1)); // Judge cancels Rod's bribe.
            //using the jugdes second action
            judge->gather();
            CHECK_THROWS(player1->bribe()); // Rod should not be able to bribe after the Judge's action.
            CHECK(player1->getCoinsCount() == 0); // Rod should have 0 coins after he tries to bribe.
        }
        SUBCASE("B: testing if player that sanctioned a judge will lose 1 extra coin") {
            Player* player1 = new PlayerTest(game, "Barak");
            Judge* judge = new Judge(game, "Rod");
            player1->addCoins(4); // Barak has 3 coins to sanction Rod.
            CHECK_NOTHROW(player1->sanction(*judge)); // Barak sanctions Rod.
            CHECK(player1->getCoinsCount() == 0); // Barak should have 0 coins after sanctioning a Judge.
        }
    }
    SUBCASE("Merchand Actions") {
        SUBCASE("A: testing the Merchand's ability to revieve one coin for each turn that he has more than 2 coins") {
            Player* player1 = new PlayerTest(game, "Barak");
            Merchand* merchand = new Merchand(game, "Rod");
            merchand->addCoins(3); // Merchand has 3 coins.
            player1->gather();
            CHECK(merchand->getCoinsCount() == 4); // Merchand should have 4 coins after the first turn.
            merchand->gather();
            CHECK(merchand->getCoinsCount() == 5); // Merchand should have 5 coins after the second turn.
            player1->gather();
            CHECK(merchand->getCoinsCount() == 6); // Merchand should have 6 coins after the third turn.
        }
        SUBCASE("B: testing if the Merchand paying 2 coins to the bank instead of 1 to the player can arrested him") {
            Player* player1 = new PlayerTest(game, "Barak");
            Merchand* merchand = new Merchand(game, "Rod");
            merchand->addCoins(3); // Merchand has 3 coins.
            CHECK_NOTHROW(player1->arrest(*merchand)); // Barak arrests Rod.
            CHECK(player1->getCoinsCount() == 0); // Barak should have 0 coins after arresting Rod since rod is a Merchand so he pays 2 coins to the bank.
            CHECK(merchand->getCoinsCount() == 1); // Merchand should have 1 coin after being arrested.
        }
    }
}
TEST_CASE("Testing if a Player is auto couping if he has 10 coins or more") {
    ex3::Game game;
    Player* player1 = new PlayerTest(game, "Barak");
    Player* player2 = new PlayerTest(game, "Rod");
    player2->addCoins(10); // Rod has 10 coins.
    player1->gather();
    CHECK_THROWS(player1->gather()); // Rod coups Barak automatically.
    CHECK(player1->isAlive() == false); // Barak should be dead after being couped.
    CHECK(player2->getCoinsCount() == 3); // Rod should have 3 coins after coup.
    CHECK(player2->getLastMove() == "coup"); // Rod's last move should be coup.
    CHECK(player2->getLastTarget() == "Barak"); // Rod's last target should be Barak.
    CHECK(player2->isAlive() == true); // Rod should be alive after coup.
    CHECK(game.getActivePlayerCount() == 1); // Only Rod should be alive after coup.
    CHECK(game.getWinner() == "Rod"); // Rod should be the winner after coup.
}
        

            


    

