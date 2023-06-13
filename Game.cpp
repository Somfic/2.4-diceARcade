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
	currentPlayer = players[((currentPlayerIndex + 1) % players.size())];
}

void Game::restart() {
	players.clear();
}

std::shared_ptr<Player> Game::getNextPlayer() {
	int currentPlayerIndex = 0;
	for (int i = 0; i < players.size(); i++) {
		if (players[i] == currentPlayer) {
			currentPlayerIndex = i;
		}
	}
	return players[((currentPlayerIndex + 1) % players.size())];
}

std::vector<std::shared_ptr<Space>> Game::getSpaces()
{
	return *spaces;
}

void Game::win(Player* p) {
	// implement GUI
	std::cout << "Player " << p->getId() << " has WON!\n";

	// each player moves to 0
	for (int i = 0; i < players.size(); i++) {
		players[i]->moveTo(0);
	}
}