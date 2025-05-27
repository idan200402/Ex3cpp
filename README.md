# Ex3 Coup Game
## Overview
This repository contains a c++ implementation of a specific implementation of the Coup game. It features an object-oriented design, 
role-based player mechanics . The GUI is implemented using SMFL, and the game logic is encapsulated in a class structure that allows for easy expansion and modification.
## Project Structure
include/ - Contains header files for the game and Player classes and interfaces.
src/ - Contains the implementation files for the game and Player classes , as well as the main function.
obj/ - Contains the compiled object files.
bin/ - Contains the compiled executable file.
assets/ - Contains the font for the GUI.
tests.cpp - Contains unit tests for the game and Player classes.
Makefile - Contains the build instructions for the project.
README.md - Documentation for the project.
## Game Design
Game is designed to be played by 2-6 players, each given a role randomly at the start of the game. Players can take actions based on their roles, and can challenge other players' actions. The game ends when only one player remains.
The main Class is `Game`, which manages the game state, player actions, and interactions. The `Player` super class is an abstract class that defines the interface for player actions , it has two pure virtual functions 'mainAbility' and 'secondaryAbility' that are implemented by the derived classes for each role. There is also PlayerTest class that is representing the player without any special abillities, it is used for testing purposes only.

## Roles Logic
for further information about the roles and their abilities, go to:
[Read The rules](https://github.com/BenjaminSaldman/CPP_EX3_25)

## Graphical User Interface
The GUI is implemented using SFML, providing a simple and intuitive interface for players to interact with the game. The GUI contains:
- A main menu for starting a new game or exiting.
- A input field for entering the amount of players and each player's name.
- A game board displaying the current state of the game, including player roles, actions, and available moves . For every valid action, the button will be turning green, and for every invalid action, the button will be turning red. After a player is eliminated, Their display looks will be changed to a gray color and they will be labelled as "Dead".
-POP-UP windows for a winner announcement.
- After five seconds, the game will automatically switch to the main menu.

## Building the Project

### Requirements
- C++ compiler (g++)
- SFML library (version 2.5 or later)
- DOCTEST library for unit testing
-valgrind for memory leak checking
### Build and Run
-To compile and run the main program
```bash
make Main
```
-To compile and run the tests
```bash
make test
```
-To clean the project (remove object files and executable)
```bash
make clean
```
-To check for memory leaks
```bash
make valgrind
```

## Testing
The unit testing is done using the DOCTEST framework. The tests are located in the `tests.cpp`. The tests cover the game logic, player actions, role abilities and edge cases. 

## Future Improvements
- Improve the user interface and user experience with more animations and visual effects.
- Add AI players to allow for single-player mode.
- Implement network multiplayer functionality for online play.

## Author
Idan Shumski - [GitHub](https://github.com/Idan200402)
gmail: idan.shumski@gmail.com
