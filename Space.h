#ifndef SPACE_H
#define SPACE_H

#include <vector>
#include "Player.h"

class Player;

class Space {
public:
	virtual void onLand(Player* p) = 0;
};

// empty space with no function
class NormalSpace : public Space {
public:
	NormalSpace();

	void onLand(Player* p) override;
};

// class GooseSpace inherits from Space
// it has a function that is called when a player lands on it that moves the player with the same distance again
class GooseSpace : public Space {
public:
	GooseSpace();

	void onLand(Player* p) override;
};

class BridgeSpace : public Space {
public:
	BridgeSpace();

	void onLand(Player* p) override;
};

class MazeSpace : public Space {
public:
	MazeSpace();

	void onLand(Player* p) override;
};

class InnSpace : public Space {
public:
	InnSpace();

	void onLand(Player* p) override;
};

// prison/well space
class WaitSpace : public Space {
public:
	Player* trappedPlayer = nullptr;

	WaitSpace();

	void onLand(Player* p) override;
};

class DeathSpace : public Space {
public:
	DeathSpace();

	void onLand(Player* p) override;
};

class WinSpace : public Space {
public:
	WinSpace();

	void onLand(Player* p) override;
};

class ExcessSpace : public Space {
public:
	ExcessSpace();

	void onLand(Player* p) override;
};

#endif // SPACE_H