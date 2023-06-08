#include "Game.h"
#include <vector>
#include <string>
#include "Player.h"
#include "Space.h"
#include <iostream>

// list of spaces in the game
std::vector<Space*> spaces;
// list of players in the game
std::vector<Player> players;

// constructor for the game that creates the spaces
Game::Game() {
	createSpaces();
}

// builder for the spaces
void Game::createSpaces()
{
	// create a new space with a lambda function as parameter and add it to the list of spaces
	for (int i = 0; i < 76; i++)
	{
		if (i == 0) 
		{
			NormalSpace* normal = new NormalSpace();
			spaces.push_back(normal);
			std::cout << i << "normalspace" << std::endl;
		}
		else if (i == 6)
		{
			BridgeSpace* bridge = new BridgeSpace();
			spaces.push_back(bridge);
			std::cout << i << "bridgespace" << std::endl;
		}
		else if (i == 19)
		{
			InnSpace* inn = new InnSpace();
			spaces.push_back(inn);
			std::cout << i << "innspace" << std::endl;
		}
		else if (i == 31)
		{
			WaitSpace* wait = new WaitSpace();
			spaces.push_back(wait);
			std::cout << i << "waitspace" << std::endl;
		}
		else if (i == 42)
		{
			MazeSpace* maze = new MazeSpace();
			spaces.push_back(maze);
			std::cout << i << "mazespace" << std::endl;
		}
		else if (i == 52)
		{
			WaitSpace* wait = new WaitSpace();
			spaces.push_back(wait);
			std::cout << i << "waitspace" << std::endl;
		}
		else if (i == 58)
		{
			DeathSpace* death = new DeathSpace();
			spaces.push_back(death);
			std::cout << i << "deathspace" << std::endl;
		}
		else if (i == 63 || i == 1)
		{
			WinSpace* win = new WinSpace();
			spaces.push_back(win);
			std::cout << i << "winspace" << std::endl;
		}
		else if (i > 63)
		{
			ExcessSpace* excess = new ExcessSpace();
			spaces.push_back(excess);
			std::cout << i << "escessspace" << std::endl;
		}
		else if (i % 9 == 0)
		{
			GooseSpace* goose = new GooseSpace();
			spaces.push_back(goose);
			std::cout << i << "goosespace" << std::endl;
		}
		else if (((i + 4)%9) == 0)
		{
			GooseSpace* goose = new GooseSpace();
			spaces.push_back(goose);
			std::cout << i << "goosespace" << std::endl;
		}
		else
		{
			NormalSpace* normal = new NormalSpace();
			spaces.push_back(normal);
			std::cout << i << "normalspace" << std::endl;
		}
			
	}
}

std::vector<Space*> Game::getSpaces()
{
	return spaces;
}
