// interface for the Space class.
// The Space class is a class that represents a space on the board.
// It has a function that is called when a player lands on it.

#include <vector>
#include "Player.h"
#include "Space.h"
#include "gameObject.h"

class Space{
public:
	void onLand(Player p);
};

// empty space with no function
class NormalSpace : public Space {
public:
	void onLand(Player p) {
		// do nothing
	}
};

// class GooseSpace inherits from Space
// it has a function that is called when a player lands on it that moves the player with the same distance again
class GooseSpace : public Space {
public:
	void onLand(Player p) {
		// move the player again
		p.moveSpaces(p.getRoll());
	}
};

class BridgeSpace : public Space {
public:
	void onLand(Player p) {
		p.moveTo(12);
	}
};

class MazeSpace : public Space {
public:
	void onLand(Player p) {
		p.moveTo(37);
	}
};

class InnSpace : public Space {
public:
	void onLand(Player p) {
		// TODO: implement
	}
};;

// prison/well space
class WaitSpace : public Space {
public:
	Player* trappedPlayer = nullptr;

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
	void onLand(Player p) {
		p.moveTo(0);
	}
};

class WinSpace : public Space {
public:
	void onLand(Player p) {
		//TODO: win the game
	}
};

class ExcessSpace : public Space {
public:
	void onLand(Player p) {
		p.moveTo(p.getCurrentSpace() - 63);
	}
};