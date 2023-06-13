#pragma once
#include <vector>
#include "Player.h"
#include "Space.h"
#include <string>

class Player;
class Space;

class Game
{
private:
    // ...

    // Declaration of winCallback variable
    typedef void (*WinCallback)(const std::string&);
    WinCallback winCallback;

public:
    std::shared_ptr<Player> currentPlayer;
    std::vector< std::shared_ptr<Player>> players;
    Game();
    void restart();
    void nextPlayer();
    std::shared_ptr<Player> getNextPlayer();
    std::shared_ptr<std::vector<std::shared_ptr<Space>>> spaces;
    std::vector<std::shared_ptr<Space>> getSpaces();
    void win(Player* p);

    // Set the callback function in the Game class
    void setWinCallback(WinCallback callback);
};

// Move the setWinCallback function definition inside the Game class
inline void Game::setWinCallback(WinCallback callback) {
    winCallback = callback;
}