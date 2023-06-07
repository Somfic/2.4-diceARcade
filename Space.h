#ifndef SPACE_H
#define SPACE_H

#include <vector>
#include "Player.h"

class Player;

class Space {
public:
	void onLand(Player* p);
};

#endif // SPACE_H