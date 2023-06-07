// interface for the Space class.
// The Space class is a class that represents a space on the board.
// It has a function that is called when a player lands on it.

#include <vector>
#include "Player.h"
#include "Space.h"

void Space::onLand(Player* p) {

}

// empty space with no function

NormalSpace::NormalSpace() {}

	void NormalSpace::onLand(Player* p) {
		// do nothing
	}


// class GooseSpace inherits from Space
// it has a function that is called when a player lands on it that moves the player with the same distance again

GooseSpace::GooseSpace() {}
	
	void GooseSpace::onLand(Player* p) {
		// move the player again
		p->moveSpaces(p->getRoll());
	}



BridgeSpace::BridgeSpace() {}

	void BridgeSpace::onLand(Player* p) {
		p->moveTo(12);
	}



MazeSpace::MazeSpace() {}

	void MazeSpace::onLand(Player* p) {
		p->moveTo(37);
	}



InnSpace::InnSpace() {}

	void InnSpace::onLand(Player* p) {
		// TODO: implement
	}


// prison/well space

	

WaitSpace::WaitSpace() {
		Player* trappedPlayer = nullptr;
	}

	void WaitSpace::onLand(Player* p) {
		if (trappedPlayer != nullptr) {
			trappedPlayer->untrap();
		}
		trappedPlayer = p;
		p->trap();
	}



DeathSpace::DeathSpace() {}

	void DeathSpace::onLand(Player* p) {
		p->moveTo(0);
	}



WinSpace::WinSpace() {}

	void WinSpace::onLand(Player* p) {
		//TODO: win the game
	}



ExcessSpace::ExcessSpace() {}

	void ExcessSpace::onLand(Player* p) {
		p->moveTo(p->getCurrentSpaceIndex() - 63);
	}
