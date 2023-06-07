#pragma once
#include "GameObject.h"
#include "Player.h"

class Player;

class Space : public GameObject {
public:
	int space;
	Space();
	~Space();
	virtual void onLand(Player* p);
};