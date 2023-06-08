#include "Player.h"
#include "Game.h"
#include <string>


// class player has an id, a color and a current space
Game* game;
int id;
std::string color;
std::shared_ptr<Space> currentSpace;
int lastRoll;
bool trapped;
bool reverse;
// constructor with an id and a color as parameters
Player::Player(int i, std::string c, Game* g) {
	// parameter variables
	this->id = i;
	this->color = c;
	this->game = g;

	this->currentSpace = game->getSpaces().at(0);
	this->lastRoll = 0;
	this->trapped = false;
}

// function that moves the player a certain amount of spaces
void Player::moveSpaces(int spaces) {
	if (trapped) {
		// if currently on an inn space
		if (getCurrentSpaceIndex() == 19) {
			currentSpace->onLand(this);
		} else
		{
			return;
		}
	}
	// get the index of the current space
	int index = getCurrentSpaceIndex();

	// move the player to the space with the index of the current space + the amount of spaces or - the amount of spaces if reverse is true
	if (reverse) {
		currentSpace = game->getSpaces()[index - spaces];
	}
	else {
		currentSpace = game->getSpaces()[index + spaces];
    }
	// call the onLand function of the space the player landed on
	currentSpace->onLand(this);
}

void Player::moveTo(int space) {
	if (trapped) {
		return;
	}

	currentSpace = game->getSpaces()[space];
	currentSpace->onLand(this);
}

// function that returns the current space
std::shared_ptr <Space> Player::getCurrentSpace() {
	return currentSpace;
}

void Player::roll(int roll) {
	lastRoll = roll;
	reverse = false;
	moveSpaces(roll);
}

int Player::getRoll() {
	return lastRoll;
}

void Player::trap() {
	trapped = true;
}

void Player::untrap() {
	trapped = false;
}

int Player::getCurrentSpaceIndex() {
	// iterate through game.getSpaces() and return the index of the current space
	for (int i = 0; i < game->getSpaces().size(); i++) {
		if (game->getSpaces()[i] == currentSpace) {
			return i;
		}
	}
	return -1;
}

int Player::getId() {
	return this->id;
}

void Player::reverseDirection()
{
	reverse = true;
}

bool Player::isTrapped()
{
	return trapped;
}