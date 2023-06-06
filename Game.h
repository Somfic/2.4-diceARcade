#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include "Player.h"
#include "Space.h" // Assuming there is a separate header file for the Space class

class Game
{
private:
    std::vector<Space*> spaces; // Changed to store pointers to Space objects
    std::vector<Player> players;

public:
    Game();
    void createSpaces();
    std::vector<Space*> getSpaces();
};

#endif // GAME_H
