#ifndef GAME_H
#define GAME_H

#include <vector>
#include "Player.h"
#include "Space.h"

class Player;
class Space;

class Game
{
private:
    std::vector<Space*> spaces;
    std::vector<Player> players;

public:
    Game();
    void createSpaces();
    std::vector<Space*> getSpaces();
};

#endif // GAME_H
