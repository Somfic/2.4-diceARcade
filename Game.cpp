#include "Game.h"
#include <vector>
#include <string>
#include "Space.cpp"

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
