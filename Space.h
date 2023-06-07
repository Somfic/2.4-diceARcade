#pragma once
#include "GameObject.h"
#include "Player.h"

class Player;

class Space : public GameObject {
public:
	virtual void onLand(Player* p);
};

// empty space with no function
class NormalSpace : public Space {
public:
	NormalSpace();

	void onLand(Player* p);
};

// class GooseSpace inherits from Space
// it has a function that is called when a player lands on it that moves the player with the same distance again
class GooseSpace : public Space {
public:
	GooseSpace();

	void onLand(Player* p);
};

class BridgeSpace : public Space {
public:
	BridgeSpace();

	void onLand(Player* p);
};

class MazeSpace : public Space {
public:
	MazeSpace();

	void onLand(Player* p);
};

class InnSpace : public Space {
public:
	InnSpace();

	void onLand(Player* p);
};

// prison/well space
class WaitSpace : public Space {
public:
	Player* trappedPlayer = nullptr;

	WaitSpace();

	void onLand(Player* p);
};

class DeathSpace : public Space {
public:
	DeathSpace();

	void onLand(Player* p);
};

class WinSpace : public Space {
public:
	WinSpace();

	void onLand(Player* p);
};

class ExcessSpace : public Space {
public:
	ExcessSpace();

	void onLand(Player* p);
};

#endif // SPACE_H