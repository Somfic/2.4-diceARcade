#pragma once
//#include "Game.h"
#include "Components.h"
#include <memory>
#include "Game.h"

class EnvirormentObject {
public:
	glm::vec3 postion;
	glm::vec3 rotation;
	std::string fileLocation;
};
class ObjectManager {
private:
	Game* game;
	std::shared_ptr <std::list<std::shared_ptr<GameObject>>> objectList;
	std::vector<EnvirormentObject> envirormentObjects;
public:
	ObjectManager( std::shared_ptr <std::list<std::shared_ptr<GameObject>>> objectList, std::string fileName, Game* game);
	~ObjectManager();
	void initEnvironment(std::string fileName);
	void addPlayer(std::shared_ptr<Player> player);
	void addTile(int tileNumber, std::shared_ptr<Space>space, float scale);
	void addTile(int tileNumber, std::shared_ptr<Space>space);
	void addEnvironmentObject(const std::string& fileName, glm::vec3 position, glm::vec3 rotation, float scale);
};
