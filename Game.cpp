#include "Game.h"
#include <vector>
#include <string>
#include "Player.h"
#include "Space.h"

// list of spaces in the game
std::vector<Space*> spaces;
// list of players in the game
std::vector<Player> players;

// constructor for the game that creates the spaces
Game::Game() {
	spaces = std::make_shared<std::vector<std::shared_ptr<Space>>>();
	createSpaces();
}

// builder for the spaces
void Game::createSpaces()
{
	// create a new space with a lambda function as parameter and add it to the list of spaces
	for (int i = 0; i < 76; i++)
	{
		if (i == 6)
		{
			BridgeSpace* bridge = new BridgeSpace();
			spaces.push_back(bridge);
		}
		else if (i == 19)
		{
			WaitSpace* wait = new WaitSpace();
			spaces.push_back(wait);
		}
		else if (i == 31)
		{
			InnSpace* inn = new InnSpace();
			spaces.push_back(inn);
		}
		else if (i == 42)
		{
			MazeSpace* maze = new MazeSpace();
			spaces.push_back(maze);
		}
		else if (i == 52)
		{
			WaitSpace* wait = new WaitSpace();
			spaces.push_back(wait);
		}
		else if (i == 58)
		{
			DeathSpace* death = new DeathSpace();
			spaces.push_back(death);
		}
		else if (i == 63)
		{
			WinSpace* win = new WinSpace();
			spaces.push_back(win);
		}
		else if (i > 63)
		{
			ExcessSpace* excess = new ExcessSpace();
			spaces.push_back(excess);
		}
		else if (i % 8 == 0)
		{
			GooseSpace* goose = new GooseSpace();
			spaces.push_back(goose);
		}
		else if ((i % 8 - 4) == 0)
		{
			GooseSpace* goose = new GooseSpace();
			spaces.push_back(goose);
		}
		else
		{
			NormalSpace* normal = new NormalSpace();
			spaces.push_back(normal);
		}
			
	}
}

std::vector<Space*> Game::getSpaces()
{
	return spaces;
}
