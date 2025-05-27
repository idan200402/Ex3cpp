#include "../include/RoleFactory.hpp"
#include "../include/Baron.hpp"
#include "../include/General.hpp"
#include "../include/Governor.hpp"
#include "../include/Judge.hpp"
#include "../include/Merchand.hpp"
#include "../include/Spy.hpp"
#include <stdexcept>

namespace ex3 {
    // RoleFactory class is responsible for creating Player objects based on the specified role.
    //its encupsulated in a static method createRole.
    Player* RoleFactory::createRole(Game& game, const std::string& name , const std::string& role) {
        if (role == "Baron") {
            return new Baron(game, name);
        } else if (role == "General") {
            return new General(game, name);
        } else if (role == "Governor") {
            return new Governor(game, name);
        } else if (role == "Judge") {
            return new Judge(game, name);
        } else if (role == "Merchand") {
            return new Merchand(game, name);
        } else if (role == "Spy") {
            return new Spy(game, name);
        } else {
            // If the role does not match any known roles, throw an exception.
            throw std::invalid_argument("this role: " + role + " does not exist");
        }
    }
}