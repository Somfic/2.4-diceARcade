#include "ObjectManager.h"
ObjectManager::ObjectManager(Game& game, std::shared_ptr <std::list<std::shared_ptr<GameObject>>> objectList) {
	this->game = game;
	this->objectList = objectList;
}
ObjectManager::~ObjectManager() {

}
void ObjectManager::initEnvironment() {

}
void ObjectManager::addPlayer(int i) {
	std::shared_ptr<Player> player = std::make_shared<Player>(i,"groen", game);
	player->addComponent(std::make_shared<ModelComponent>("models/steve/steve.obj"));
	player->addComponent(std::make_shared<MoveToComponent>());
	objectList->push_back(player);
}
void ObjectManager::addTile(int tileNumber, std::shared_ptr <Space> space) {
	space->addComponent(std::make_shared<ModelComponent>("models/Tiles/tile.obj"));
	objectList->push_back(space);
}
void ObjectManager::addEnvironmentObject(const std::string& fileName, glm::vec3 position, glm::vec3 rotation) {
	std::shared_ptr<GameObject> object = std::make_shared<GameObject>();
	object->addComponent(std::make_shared<ModelComponent>(fileName));
	object->position = position;
	object->rotation = rotation;
	objectList->push_back(object);
}
