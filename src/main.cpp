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

using namespace ex3;

const std::vector<std::string> publicMethods = {
    "gather", "tax", "bribe", "arrest", "sanction", "coup", "mainAbility", "secondAbility"
};

struct Button {
    sf::RectangleShape rect;
    sf::Text label;
    std::string action;
};

int main() {
    Game game;
    std::vector<std::string> roles = {"Spy", "Governor", "Merchand", "Judge", "Baron", "General"};
    std::vector<std::string> playerNames;
    int playerCount = 0;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Coup: Enter Players");
    sf::Font font;
    if (!font.loadFromFile("assets/arial.ttf")) {
        std::cerr << "❌ Failed to load font" << std::endl;
        return 1;
    }

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
                                window.close(); // Done collecting names
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

    // Create roles after window closes
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for (const std::string& name : playerNames) {
        std::string role = roles[std::rand() % roles.size()];
        try {
            RoleFactory::createRole(game, name, role);
            std::cout << "✅ Created player: " << name << " with role: " << role << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "❌ Failed to create " << name << ": " << e.what() << std::endl;
        }
    }

    auto players = game.getPlayers();
    window.create(sf::VideoMode(1200, 160 * playerCount), "Coup: Player Interface");

    std::vector<std::vector<Button>> playerButtons(playerCount);

    for (int i = 0; i < playerCount; ++i) {
        float yOffset = i * 160;
        for (size_t j = 0; j < publicMethods.size(); ++j) {
            Button btn;
            btn.action = publicMethods[j];
            btn.rect.setSize(sf::Vector2f(120, 30));
            btn.rect.setPosition(200 + j * 130, yOffset + 20);
            btn.rect.setFillColor(sf::Color::Blue);

            btn.label.setFont(font);
            btn.label.setString(publicMethods[j]);
            btn.label.setCharacterSize(16);
            btn.label.setFillColor(sf::Color::White);
            btn.label.setPosition(btn.rect.getPosition().x + 10, btn.rect.getPosition().y + 5);

            playerButtons[i].push_back(btn);
        }
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                for (int i = 0; i < playerCount; ++i) {
                    for (Button& btn : playerButtons[i]) {
                        if (btn.rect.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                            try {
                                if (btn.action == "gather") players[i]->gather();
                                else if (btn.action == "tax") players[i]->tax();
                                else if (btn.action == "bribe") players[i]->bribe();
                                else if (btn.action == "mainAbility") players[i]->mainAbility(nullptr);
                                else if (btn.action == "secondAbility") players[i]->secondaryAbility(nullptr);
                                else if (btn.action == "arrest") players[i]->arrest(*players[(i + 1) % playerCount]);
                                else if (btn.action == "sanction") players[i]->sanction(*players[(i + 1) % playerCount]);
                                else if (btn.action == "coup") players[i]->coup(*players[(i + 1) % playerCount]);

                                btn.rect.setFillColor(sf::Color::Green);
                            } catch (const std::exception& e) {
                                std::cerr << "❌ " << players[i]->getName() << " failed to " << btn.action << ": " << e.what() << std::endl;
                                btn.rect.setFillColor(sf::Color::Red);
                                std::this_thread::sleep_for(std::chrono::milliseconds(300));
                                btn.rect.setFillColor(sf::Color::Blue);
                            }
                        }
                    }
                }
            }
        }

        window.clear(sf::Color(30, 30, 30));
        for (int i = 0; i < playerCount; ++i) {
            sf::Text playerLabel(players[i]->getName(), font, 20);
            playerLabel.setPosition(20, i * 160 + 20);
            window.draw(playerLabel);

            for (const auto& btn : playerButtons[i]) {
                window.draw(btn.rect);
                window.draw(btn.label);
            }
        }
        window.display();
    }

    return 0;
}