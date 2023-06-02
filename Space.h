#ifndef SPACE_H
#define SPACE_H

#include <vector>
#include "Player.h"

class Space : public GameObject{
public:
	GameObject railing;
	void onLand(Player p);
};

#endif // SPACE_H