#include "Player.h"
#include "Game.h"
#include <string>
#include "Space.cpp"

// class player has an id, a color and a current space
class Player
{
	Game game;
	int id;
	std::string color;
	Space currentSpace;
	int lastRoll;
	bool trapped;
	// constructor with an id and a color as parameters
	Player(int i, std::string c, Game g) {
		// parameter variables
		game = g;
		id = i;
		color = c;
		
		// default variables
		currentSpace = game.getSpaces()[0];
		lastRoll = 0;
		trapped = false;
	}

	// function that moves the player a certain amount of spaces
	void moveSpaces(int spaces) {
		if (trapped) {
			return;
		}
		// get the index of the current space
		int index = std::find(game.getSpaces().begin(), game.getSpaces().end(), currentSpace) - game.getSpaces().begin();

		// move the player to the space with the index of the current space + the amount of spaces
		currentSpace = game.getSpaces()[index + spaces];

		// call the onLand function of the space the player landed on
		currentSpace.onLand(*this);
	}

	void moveTo(int space) {
		if (trapped) {
			return;
		}

		currentSpace = game.getSpaces()[space];
		currentSpace.onLand(*this);
	}

	// function that returns the current space
	Space getCurrentSpace() {
		return currentSpace;
	}

	void roll(int roll) {
		lastRoll = roll;
		moveSpaces(roll);
	}

	int getRoll() {
		return lastRoll;
	}

	void trap() {
		trapped = true;
	}

	void untrap() {
		trapped = false;
	}
};