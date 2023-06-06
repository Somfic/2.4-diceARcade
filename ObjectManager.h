#pragma once
//#include "Game.h"
#include "Components.h"
class ObjectManager {
private:
	//Game& game;
	std::shared_ptr <std::list<std::shared_ptr<GameObject>>> objectList;
	std::vector<glm::vec4> tileInfo;
public:
	ObjectManager( std::shared_ptr <std::list<std::shared_ptr<GameObject>>> objectList); //Game &game,
	~ObjectManager();
	void initEnvironment();
	void addPlayer(int i);
	void addTile(int tileNumber);// ,std::shared_ptr <Space> space);
	void addEnvironmentObject(const std::string& fileName, glm::vec3 position, glm::vec3 rotation);
};