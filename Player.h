#ifndef PLAYER_H
#define PLAYER_H

#include "Game.h"
#include <string>
#include "Space.h"

class Game;
class Space;

class Player : public GameObject{
private:
	Game* game;
	int id;
	std::string color;
	std::shared_ptr<Space> currentSpace;
	int lastRoll;
	bool trapped;

public:
	Player(int i, std::string c, Game* game);
	void moveSpaces(int spaces);
	void moveTo(int space);
	std::shared_ptr <Space> getCurrentSpace();
	int getCurrentSpaceIndex();
	void roll(int roll);
	int getRoll();
	void trap();
	void untrap();

};
#endif // PLAYER_H