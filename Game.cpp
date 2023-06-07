#include "Game.h"
#include <vector>
#include <string>
#include "Player.h"
#include "Space.h"

// list of spaces in the game
std::vector<Space> spaces;
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
		if (i == 6)
			spaces.push_back(BridgeSpace());
		else if (i == 19)
			spaces.push_back(WaitSpace());
		else if (i == 31)
			spaces.push_back(InnSpace());
		else if (i == 42)
			spaces.push_back(MazeSpace());
		else if (i == 52)
			spaces.push_back(WaitSpace());
		else if (i == 58)
			spaces.push_back(DeathSpace());
		else if (i == 63)
			spaces.push_back(WinSpace());
		else if (i > 63)
			spaces.push_back(ExcessSpace());
		else if (i % 8 == 0)
			spaces.push_back(GooseSpace());
		else if ((i & 8 - 4) == 0)
			spaces.push_back(GooseSpace());
		else
			spaces.push_back(NormalSpace());

	}
}

std::vector<Space*> Game::getSpaces()
{
	std::vector<Space*> spacePointers;
	for (int i = 0; i < spaces.size(); i++)
	{
		spacePointers.push_back(&spaces[i]);
	}
	return spacePointers;
}
