// interface for the Space class.
// The Space class is a class that represents a space on the board.
// It has a function that is called when a player lands on it.

#include <vector>
#include <iostream>
#include "Player.h"
#include "Space.h"

void Space::onLand(Player* p) {
	std::cout << "Player " << p->getId() << " is on space\n";
}

// empty space with no function

NormalSpace::NormalSpace() {}

void NormalSpace::onLand(Player* p) {
	// do nothing
	std::cout << "Player " << p->getId() << " is on normal space\n";
}


// class GooseSpace inherits from Space
// it has a function that is called when a player lands on it that moves the player with the same distance again

GooseSpace::GooseSpace() {}

void GooseSpace::onLand(Player* p) {
	std::cout << "Player " << p->getId() << " is on goose space\n";
	// move the player again
	p->moveSpaces(p->getRoll());
}



BridgeSpace::BridgeSpace() {}

void BridgeSpace::onLand(Player* p) {
	std::cout << "Player " << p->getId() << " is on bridge space\n";
	p->moveTo(12);
}



MazeSpace::MazeSpace() {}

void MazeSpace::onLand(Player* p) {
	std::cout << "Player " << p->getId() << " is on maze space\n";
	p->moveTo(37);
}



InnSpace::InnSpace() {}

void InnSpace::onLand(Player* p) {
	// TODO: implement
	std::cout << "Player " << p->getId() << " is on inn space\n";
}


// prison/well space



WaitSpace::WaitSpace() {
	Player* trappedPlayer = nullptr;
}

void WaitSpace::onLand(Player* p) {
	std::cout << "Player " << p->getId() << " is on wait space\n";
	if (trappedPlayer != nullptr) {
		trappedPlayer->untrap();
	}
	trappedPlayer = p;
	p->trap();
}



DeathSpace::DeathSpace() {}

void DeathSpace::onLand(Player* p) {
	std::cout << "Player " << p->getId() << " is on death space\n";
	p->moveTo(0);
}



WinSpace::WinSpace() {}

void WinSpace::onLand(Player* p) {
	std::cout << "Player " << p->getId() << " is on win space\n";
	//TODO: win the game
	
}



ExcessSpace::ExcessSpace() {}

void ExcessSpace::onLand(Player* p) {
	std::cout << "Player " << p->getId() << " is on excess space\n";
	p->moveTo(63 - (p->getCurrentSpaceIndex() - 63));
}