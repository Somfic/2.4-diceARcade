#include "Game.h"
#include <vector>
#include <string>

class Game
{
	// list of spaces in the game
	std::vector<Space> spaces;
	// list of players in the game
	std::vector<Player> players;


	Game() {
		createSpaces();
	}

	// builder for the spaces
	void createSpaces() {
		// create a new space with a lambda function as parameter and add it to the list of spaces
		for (int i = 0; i < 64; i++) {

			if (i % 8 == 0) {
				spaces.push_back(Space([]() {
					}));
			}



			spaces.push_back(Space());
		}
	}

	std::vector<Space> getSpaces() {
		return spaces;
	}
};


// class player has an id, a color and a current space
class Player
{
	Game game;
	int id;
	std::string color;
	Space currentSpace;
	// constructor with an id and a color as parameters
	Player(int i, std::string c, Game g) {
		game = g;
		// set the id and the color
		id = i;
		color = c;
		// set the current space to the first space
		currentSpace = game.getSpaces[0];
	}
};