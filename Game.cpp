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
	spaces = std::make_shared<std::vector<std::shared_ptr<Space>>>();
	//createSpaces();
}

void Game::nextPlayer() {
	int currentPlayerIndex = 0;
	for (int i = 0; i < players.size(); i++) {
		if (players[i] == currentPlayer) {
			currentPlayerIndex = i;
		}
	}
	std::cout << currentPlayerIndex << "..." << ((currentPlayerIndex + 1) % (players.size())) << std::endl;
	currentPlayer = players[((currentPlayerIndex + 1) % players.size())];
}

std::vector<std::shared_ptr<Space>> Game::getSpaces()
{
	return *spaces;
}
