#pragma once
#include <vector>
#include "Player.h"
#include "Space.h"

class Player;
class Space;

class Game
{
private:
    
   

public:
    std::vector< std::shared_ptr<Player>> players;
    Game();
    std::shared_ptr<std::vector<std::shared_ptr<Space>>> spaces;
    void createSpaces();
    std::vector<std::shared_ptr<Space>> getSpaces();
};

