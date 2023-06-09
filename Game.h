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
    std::shared_ptr<Player> currentPlayer;
    std::vector< std::shared_ptr<Player>> players;
    Game();
    void nextPlayer();
    std::shared_ptr<Player> getNextPlayer();
    std::shared_ptr<std::vector<std::shared_ptr<Space>>> spaces;
    std::vector<std::shared_ptr<Space>> getSpaces();
    void win(Player* p);
};

