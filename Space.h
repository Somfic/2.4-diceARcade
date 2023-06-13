#pragma once
#include "GameObject.h"
#include "Player.h"
#include <iostream>

class Player;

class Space : public GameObject {
public:
	Space();
	int typeId;
	virtual void onLand(Player* p);
	virtual void addName(std::string& dest);
};

// empty space with no function
class NormalSpace : public Space {
public:
	NormalSpace();
	void addName(std::string& dest) override;
	void onLand(Player* p) override;
};

// class GooseSpace inherits from Space
// it has a function that is called when a player lands on it that moves the player with the same distance again
class GooseSpace : public Space {
public:
	GooseSpace();
	void addName(std::string& dest) override;
	void onLand(Player* p) override;
};

class BridgeSpace : public Space {
public:
	BridgeSpace();
	void addName(std::string& dest) override;
	void onLand(Player* p) override;
};

class MazeSpace : public Space {
public:
	MazeSpace();
	void addName(std::string& dest) override;
	void onLand(Player* p) override;
};

class InnSpace : public Space {
public:
	InnSpace();
	void addName(std::string& dest) override;
	void onLand(Player* p) override;
};

// prison/well space
class WaitSpace : public Space {
public:
	Player* trappedPlayer = nullptr;
	void addName(std::string& dest) override;
	WaitSpace();

	void onLand(Player* p) override;
};

class DeathSpace : public Space {
public:
	DeathSpace();
	void addName(std::string& dest) override;
	void onLand(Player* p) override;
};

class WinSpace : public Space {
public:
	WinSpace();
	void addName(std::string& dest) override;
	void onLand(Player* p) override;
};

class ExcessSpace : public Space {
public:
	ExcessSpace();
	void addName(std::string& dest) override;
	void onLand(Player* p) override;
};

// SPACE_H