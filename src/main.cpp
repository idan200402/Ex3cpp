// idan.shumski@gmail.com

#include <SFML/Graphics.hpp>
#include "../include/Game.hpp"
#include "../include/RoleFactory.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <unordered_map>
#include <thread>
#include <chrono>
#include <optional>

using namespace ex3;
//List of the public methods available to players , it will be presented as buttons in the game interface.
const std::vector<std::string> publicMethods = {
    "gather", "tax", "bribe", "arrest", "sanction", "coup", "mainAbility", "secondAbility"
};
/**
 * @struct Button
 * @brief Represents a button in the game interface.
 * Contains a rectangle shape, label text, action string, original color, and color change time.
 */
struct Button {
    sf::RectangleShape rect;
    sf::Text label;
    std::string action;
    sf::Color originalColor = sf::Color::Blue;
    std::chrono::steady_clock::time_point colorChangeTime;
    bool isColorTemporary = false;
};
/**
 * @struct TargetButton
 * @brief Represents a button for selecting a target player.
 * when a player chooses to perform an action that requires a target he required to select a target player.
 */
struct TargetButton {
    sf::RectangleShape rect;
    sf::Text label;
    int targetIndex = -1;
};
/**
 * @brief Displays the welcome screen with options to start or exit the game.
 * @param window The SFML render window to display the welcome screen.
 * @param font The font used for text rendering.
 * @return True if the user chooses to start the game, false if they choose to exit.
 */
bool showWelcomeScreen(sf::RenderWindow &window, sf::Font &font) {
    sf::Text title("Welcome to COUP", font, 40);
    title.setPosition(500, 150);
    title.setFillColor(sf::Color::Cyan);

    sf::RectangleShape startBtn(sf::Vector2f(300, 60));
    startBtn.setPosition(550, 300);
    startBtn.setFillColor(sf::Color::Green);
    sf::Text startText("Start Game", font, 28);
    startText.setPosition(590, 315);
    startText.setFillColor(sf::Color::Black);

    sf::RectangleShape exitBtn(sf::Vector2f(300, 60));
    exitBtn.setPosition(550, 400);
    exitBtn.setFillColor(sf::Color::Red);
    sf::Text exitText("Exit Game", font, 28);
    exitText.setPosition(600, 415);
    exitText.setFillColor(sf::Color::Black);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                if (startBtn.getGlobalBounds().contains(mousePos)) {
                    return true;
                }
                if (exitBtn.getGlobalBounds().contains(mousePos)) {
                    window.close();
                    return false;
                }
            }
        }
        window.clear(sf::Color::Black);
        window.draw(title);
        window.draw(startBtn);
        window.draw(startText);
        window.draw(exitBtn);
        window.draw(exitText);
        window.display();
    }
    return false;
}
/**
 * @brief Main function to run the Coup game.
 * Initializes the game window, loads resources, and manages the game loop.
 * @return Exit code of the program.
 */
int main() {
main_loop:
    sf::RenderWindow window(sf::VideoMode(1600, 900), "Coup: Welcome");
    sf::Font font;
    if (!font.loadFromFile("assets/arial.ttf")) {
        std::cerr << "❌ Failed to load font" << std::endl;
        return 1;
    }

    if (!showWelcomeScreen(window, font)) return 0;

    window.create(sf::VideoMode(1600, 900), "Coup: Enter Players");
    // Load resources and initialize game state
    Game game;
    std::vector<std::string> roles = {"Spy", "Governor", "Merchand", "Judge", "Baron", "General"};
    std::vector<std::string> playerNames;
    int playerCount = 0;
    sf::Text prompt("Enter number of players (2-6):", font, 24);
    prompt.setPosition(40, 40);
    prompt.setFillColor(sf::Color::White);

    sf::Text inputText("", font, 24);
    inputText.setPosition(40, 100);
    inputText.setFillColor(sf::Color::Cyan);

    std::string inputBuffer;
    bool collectingNames = false;
    int currentNameIndex = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b') {
                    if (!inputBuffer.empty()) inputBuffer.pop_back();
                } else if (event.text.unicode == '\r') {
                    if (!collectingNames) {
                        try {
                            playerCount = std::stoi(inputBuffer);
                            if (playerCount < 2 || playerCount > 6) throw std::invalid_argument("Range");
                            playerNames.resize(playerCount);
                            collectingNames = true;
                            prompt.setString("Enter name for player 1:");
                        } catch (...) {
                            prompt.setString("❌ Must enter number 2–6:");
                        }
                    } else {
                        if (!inputBuffer.empty()) {
                            playerNames[currentNameIndex] = inputBuffer;
                            currentNameIndex++;
                            if (currentNameIndex < playerCount) {
                                prompt.setString("Enter name for player " + std::to_string(currentNameIndex + 1) + ":");
                            } else {
                                window.close();
                            }
                        }
                    }
                    inputBuffer.clear();
                } else if (event.text.unicode < 128) {
                    inputBuffer += static_cast<char>(event.text.unicode);
                }
            }
        }

        inputText.setString(inputBuffer);
        window.clear(sf::Color::Black);
        window.draw(prompt);
        window.draw(inputText);
        window.display();
    }

    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for (const std::string& name : playerNames) {
        std::string role = roles[std::rand() % roles.size()];
        try {
            RoleFactory::createRole(game, name, role);
        } catch (const std::exception& e) {
            std::cerr << "❌ Failed to create " << name << ": " << e.what() << std::endl;
        }
    }

    auto players = game.getPlayers();
    window.create(sf::VideoMode(1600, 160 * playerCount), "Coup: Player Interface");

    std::vector<std::vector<Button>> playerButtons(playerCount);
    std::vector<TargetButton> targetButtons(playerCount);

    for (int i = 0; i < playerCount; ++i) {
        float yOffset = i * 160;
        for (size_t j = 0; j < publicMethods.size(); ++j) {
            Button btn;
            btn.action = publicMethods[j];
            btn.rect.setSize(sf::Vector2f(120, 30));
            btn.rect.setPosition(300 + j * 130, yOffset + 20);
            btn.rect.setFillColor(btn.originalColor);

            btn.label.setFont(font);
            btn.label.setString(publicMethods[j]);
            btn.label.setCharacterSize(16);
            btn.label.setFillColor(sf::Color::White);
            btn.label.setPosition(btn.rect.getPosition().x + 10, btn.rect.getPosition().y + 5);

            playerButtons[i].push_back(btn);
        }

        TargetButton tBtn;
        tBtn.rect.setSize(sf::Vector2f(100, 25));
        tBtn.rect.setPosition(20, yOffset + 50);
        tBtn.rect.setFillColor(sf::Color(100, 100, 255));
        tBtn.label.setFont(font);
        tBtn.label.setString("Select");
        tBtn.label.setCharacterSize(14);
        tBtn.label.setFillColor(sf::Color::White);
        tBtn.label.setPosition(tBtn.rect.getPosition().x + 10, tBtn.rect.getPosition().y + 3);
        tBtn.targetIndex = i;
        targetButtons[i] = tBtn;
    }

    std::string currentTurnPlayer = "";
    std::optional<std::string> waitingForTargetAction;
    int actingPlayerIndex = -1;

    while (window.isOpen()) {
        try { currentTurnPlayer = game.turn(); } catch (...) {}

        try {
            std::string winner = game.getWinner();
            sf::RenderWindow endWindow(sf::VideoMode(600, 200), "Game Over");
            sf::Text winText("The Coup Game Ended.\nThe winner is: " + winner, font, 28);
            winText.setPosition(40, 70);
            winText.setFillColor(sf::Color::Yellow);
            auto start = std::chrono::steady_clock::now();
            while (endWindow.isOpen()) {
                sf::Event e;
                while (endWindow.pollEvent(e)) {
                    if (e.type == sf::Event::Closed) endWindow.close();
                }
                if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count() > 5)
                    endWindow.close();
                endWindow.clear(sf::Color::Black);
                endWindow.draw(winText);
                endWindow.display();
            }
            goto main_loop;
        } catch (...) {}

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                if (waitingForTargetAction) {
                    for (int target = 0; target < playerCount; ++target) {
                        if (targetButtons[target].rect.getGlobalBounds().contains(mousePos)) {
                            try {
                                Player* actor = players[actingPlayerIndex];
                                Player* victim = players[target];
                                std::string action = *waitingForTargetAction;
                                if (action == "arrest") actor->arrest(*victim);
                                else if (action == "sanction") actor->sanction(*victim);
                                else if (action == "coup") actor->coup(*victim);
                                else if (action == "mainAbility") actor->mainAbility(victim);
                                else if (action == "secondAbility") actor->secondaryAbility(victim);
                                auto& btn = playerButtons[actingPlayerIndex][std::distance(publicMethods.begin(), std::find(publicMethods.begin(), publicMethods.end(), action))];
                                btn.rect.setFillColor(sf::Color::Green);
                                btn.colorChangeTime = std::chrono::steady_clock::now();
                                btn.isColorTemporary = true;

                            } catch (...) {
                                auto& btn = playerButtons[actingPlayerIndex][std::distance(publicMethods.begin(), std::find(publicMethods.begin(), publicMethods.end(), *waitingForTargetAction))];
                                btn.rect.setFillColor(sf::Color::Red);
                                btn.colorChangeTime = std::chrono::steady_clock::now();
                                btn.isColorTemporary = true;

                            }
                            waitingForTargetAction.reset();
                        }
                    }
                } else {
                    for (int i = 0; i < playerCount; ++i) {
                        for (Button& btn : playerButtons[i]) {
                            if (btn.rect.getGlobalBounds().contains(mousePos)) {
                                try {
                                    if (btn.action == "gather") players[i]->gather();
                                    else if (btn.action == "tax") players[i]->tax();
                                    else if (btn.action == "bribe") players[i]->bribe();
                                    else if (btn.action == "arrest" || btn.action == "sanction" || btn.action == "coup" || btn.action == "mainAbility" || btn.action == "secondAbility") {
                                        waitingForTargetAction = btn.action;
                                        actingPlayerIndex = i;
                                        continue;
                                    }
                                    btn.rect.setFillColor(sf::Color::Green);
                                    btn.colorChangeTime = std::chrono::steady_clock::now();
                                    btn.isColorTemporary = true;
                                } catch (...) {
                                    btn.rect.setFillColor(sf::Color::Red);
                                    btn.colorChangeTime = std::chrono::steady_clock::now();
                                    btn.isColorTemporary = true;
                                }
                            }
                        }
                    }
                }
            }
        }

        window.clear(sf::Color(30, 30, 30));
        for (int i = 0; i < playerCount; ++i) {
            std::string playerInfo = players[i]->getName() + " |  " + players[i]->getRole() + " |  " + std::to_string(players[i]->getCoinsCount());
            sf::Text playerLabel(playerInfo, font, 20);
            playerLabel.setPosition(20, i * 160 + 20);

            if (!players[i]->isAlive()) {
                playerLabel.setFillColor(sf::Color(120, 120, 120));
                playerLabel.setStyle(sf::Text::Italic);
                targetButtons[i].label.setString("Dead");
                targetButtons[i].rect.setFillColor(sf::Color(80, 80, 80));
            } else if (players[i]->getName() == currentTurnPlayer) {
                playerLabel.setStyle(sf::Text::Bold);
                playerLabel.setFillColor(sf::Color::Yellow);
            } else {
                playerLabel.setStyle(sf::Text::Regular);
                playerLabel.setFillColor(sf::Color::White);
            }

            window.draw(playerLabel);
            window.draw(targetButtons[i].rect);
            window.draw(targetButtons[i].label);

            for (Button& btn : playerButtons[i]) {
                window.draw(btn.rect);
                window.draw(btn.label);
            }
        }
        auto now = std::chrono::steady_clock::now();
        for (int i = 0; i < playerCount; ++i) {
            for (Button& btn : playerButtons[i]) {
                if (btn.isColorTemporary) {
                    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - btn.colorChangeTime).count();
                    if (elapsed > 300) {
                        btn.rect.setFillColor(btn.originalColor);
                        btn.isColorTemporary = false;
                    }
                }
            }
        }
        

        window.display();
    }

    return 0;
}
