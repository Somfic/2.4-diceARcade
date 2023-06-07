#include "Space.h"
Space::Space() {

// empty space with no function
class NormalSpace : public Space {
public:
	NormalSpace() {}

	void onLand(Player p) {
		// do nothing
	}
};

// class GooseSpace inherits from Space
// it has a function that is called when a player lands on it that moves the player with the same distance again
class GooseSpace : public Space {
public:
	GooseSpace() {}
	
	void onLand(Player p) {
		// move the player again
		p.moveSpaces(p.getRoll());
	}
};

class BridgeSpace : public Space {
public:
	BridgeSpace() {}

	void onLand(Player p) {
		p.moveTo(12);
	}
};

class MazeSpace : public Space {
public:
	MazeSpace() {}

	void onLand(Player p) {
		p.moveTo(37);
	}
};

class InnSpace : public Space {
public:
	InnSpace() {}

	void onLand(Player p) {
		// TODO: implement
	}
};;

// prison/well space
class WaitSpace : public Space {
public:
	Player* trappedPlayer = nullptr;

	WaitSpace() {}

	void onLand(Player p) {
		if (trappedPlayer != nullptr) {
			trappedPlayer->untrap();
		}
		trappedPlayer = &p;
		p.trap();
	}
};

class DeathSpace : public Space {
public:
	DeathSpace() {}

	void onLand(Player p) {
		p.moveTo(0);
	}
};

class WinSpace : public Space {
public:
	WinSpace() {}

	void onLand(Player p) {
		//TODO: win the game
	}
};

class ExcessSpace : public Space {
public:
	ExcessSpace() {}

	void onLand(Player p) {
		p.moveTo(p.getCurrentSpaceIndex() - 63);
	}
};