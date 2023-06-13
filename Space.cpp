// interface for the Space class.
// The Space class is a class that represents a space on the board.
// It has a function that is called when a player lands on it.

#include <vector>
#include <iostream>
#include "Player.h"
#include "Space.h"
Space::Space() {
}
void Space::onLand(Player* p) {
	std::cout << "On space\n";
}

void Space::addName(std::string& dest) {
	dest += "Space";
}

// empty space with no function

NormalSpace::NormalSpace() {}

void NormalSpace::onLand(Player* p) {
	// do nothing
	std::cout << "On normal space\n";
}

void NormalSpace::addName(std::string& dest) {
	dest += "Normal Space";
}


// class GooseSpace inherits from Space
// it has a function that is called when a player lands on it that moves the player with the same distance again

GooseSpace::GooseSpace() {}

void GooseSpace::onLand(Player* p) {
	std::cout << "On goose space\n";
	// move the player again
	p->moveSpaces(p->getRoll());
}

void GooseSpace::addName(std::string& dest) {
	dest += "Goose Space";
}

BridgeSpace::BridgeSpace() {}

void BridgeSpace::onLand(Player* p) {
	std::cout << "On bridge space\n";
	p->moveTo(12);
}

void BridgeSpace::addName(std::string& dest) {
	dest += "Bridge Space";
}

MazeSpace::MazeSpace() {}

void MazeSpace::onLand(Player* p) {
	std::cout << "On maze space\n";
	p->moveTo(37);
}

void MazeSpace::addName(std::string& dest) {
	dest += "Maze Space";
}

InnSpace::InnSpace() {}

void InnSpace::onLand(Player* p) {
	std::cout << "On inn space\n";
	if (p->isTrapped()) {
		p->untrap();
	}
	else {
		p->trap();
	}
}

void InnSpace::addName(std::string& dest) {
	dest += "Inn Space";
}
// prison/well space



WaitSpace::WaitSpace() {
	Player* trappedPlayer = nullptr;
}

void WaitSpace::onLand(Player* p) {
	std::cout << "On wait space\n";
	if (trappedPlayer != nullptr) {
		trappedPlayer->untrap();
	}
	trappedPlayer = p;
	p->trap();
}

void WaitSpace::addName(std::string& dest) {
	dest += "Wait Space";
}

DeathSpace::DeathSpace() {}

void DeathSpace::onLand(Player* p) {
	std::cout << "On death space\n";
	p->moveTo(0);
}

void DeathSpace::addName(std::string& dest) {
	dest += "Death Space";
}

WinSpace::WinSpace() {}

void WinSpace::onLand(Player* p) {
	std::cout << "On win space\n";
	p->win();
}

void WinSpace::addName(std::string& dest) {
	dest += "Win Space";
}

ExcessSpace::ExcessSpace() {}

void ExcessSpace::onLand(Player* p) {
	std::cout << "On excess space\n";
	p->moveTo(63 - (p->getCurrentSpaceIndex() - 63));
}

void ExcessSpace::addName(std::string& dest) {
	dest += "Excess Space";
}