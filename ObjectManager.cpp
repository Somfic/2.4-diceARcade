#include "ObjectManager.h"
ObjectManager::ObjectManager( std::shared_ptr <std::list<std::shared_ptr<GameObject>>> objectList) //Game& game,
{ 
	//this->game = game;
	this->objectList = objectList;
	initEnvironment();
}
ObjectManager::~ObjectManager() {

}
void ObjectManager::initEnvironment() {
	tileInfo.push_back(glm::vec4(0, 0, 0, 0));
	tileInfo.push_back(glm::vec4(0, 2, 0, 2));
	tileInfo.push_back(glm::vec4(0, 4, 0, 0));
	tileInfo.push_back(glm::vec4(0, 6, 1, 1));
	tileInfo.push_back(glm::vec4(2, 6, 3, 1));
	tileInfo.push_back(glm::vec4(2, 8, 2, 0));
	addTile(0 );// ,std::make_shared <Space>());
	addTile(1 );// ,std::make_shared <Space>());
	addTile(2 );// ,std::make_shared <Space>());
	addTile(3 );// ,std::make_shared <Space>());
	addTile(4);// ,std::make_shared <Space>());
	addTile(5);// ,std::make_shared <Space>());
}
void ObjectManager::addPlayer(int i) {
	/*std::shared_ptr<Player> player = std::make_shared<Player>(i,"groen", game);
	player->addComponent(std::make_shared<ModelComponent>("models/steve/steve.obj"));
	player->addComponent(std::make_shared<MoveToComponent>());
	objectList->push_back(player);*/
}
void ObjectManager::addTile(int tileNumber)//void ObjectManager::addTile(int tileNumber, std::shared_ptr <Space> space)
{
	//remove
	std::shared_ptr<GameObject> space = std::make_shared <GameObject>();
	//
	space->addComponent(std::make_shared<ModelComponent>("models/Tiles/tile.obj"));
	space->position = glm::vec3(tileInfo[tileNumber].x, 0, tileInfo[tileNumber].y);
	space->rotation = glm::vec3(0, 0, 0);
	objectList->push_back(space);
	std::shared_ptr<GameObject> railing = std::make_shared<GameObject>();
	if (tileInfo[tileNumber].w == 1) {
		railing->addComponent(std::make_shared<ModelComponent>("models/Tiles/rail_curved.obj"));
	}
	else if (tileInfo[tileNumber].w == 2) {
		railing->addComponent(std::make_shared<ModelComponent>("models/Tiles/rail_straight.obj"));
	}
	else {
		railing->addComponent(std::make_shared<ModelComponent>("models/Tiles/rail_straight.obj"));
		std::shared_ptr<GameObject> railing2 = std::make_shared<GameObject>();
		railing2->addComponent(std::make_shared<ModelComponent>("models/Tiles/rail_straight.obj"));
		railing2->position = glm::vec3(tileInfo[tileNumber].x, 0, tileInfo[tileNumber].y);
		railing2->rotation = glm::vec3(0, (tileInfo[tileNumber].z + 2) * 3.14 / 2, 0);
		objectList->push_back(railing2);
		
	}
	railing->position = glm::vec3(tileInfo[tileNumber].x, 0, tileInfo[tileNumber].y);
	railing->rotation = glm::vec3(0, tileInfo[tileNumber].z * 3.14 / 2, 0);
	objectList->push_back(railing);
}
void ObjectManager::addEnvironmentObject(const std::string& fileName, glm::vec3 position, glm::vec3 rotation) {
	std::shared_ptr<GameObject> object = std::make_shared<GameObject>();
	object->addComponent(std::make_shared<ModelComponent>(fileName));
	object->position = position;
	object->rotation = rotation;
	objectList->push_back(object);
}
