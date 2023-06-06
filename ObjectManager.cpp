#include "ObjectManager.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

ObjectManager::ObjectManager(std::shared_ptr <std::list<std::shared_ptr<GameObject>>> objectList, std::string fileName) //Game& game,
{
	//this->game = game;
	this->objectList = objectList;
	initEnvironment(fileName);
}
ObjectManager::~ObjectManager() {

}

#include "ExtraString.h"

glm::vec3 stringVectorToVec3(std::vector<std::string> strings) {
	return glm::vec3(atoi(strings[0].c_str()), atoi(strings[1].c_str()), atoi(strings[2].c_str()));
}
float degToRad(float degree) {
	return degree / 180.0f * 3.14159265359f;
}

void ObjectManager::initEnvironment(std::string fileName) {
	std::cout << "Loading Environment from " << fileName << std::endl;
	std::ifstream pFile(fileName.c_str());
	if (!pFile.is_open())
	{
		std::cout << "Could not open file " << fileName << std::endl;
		return;
	}
	while (!pFile.eof())
	{
		std::string line;
		std::getline(pFile, line);
		line = cleanLine(line);
		if (line == "" || line[0] == '#') //skip empty or commented line
			continue;
		std::vector<std::string> params = split(line, " ");
		params[0] = toLower(params[0]);
		std::vector<std::string> position = split(params[1], ",");
		std::vector<std::string> rotation = split(params[2], ",");

		if (params[0] == "t") {
			std::string type = params[3];
			int typeId;
			if (type == "str")
				typeId = 0;
			else if (type == "cor")
				typeId = 1;
			else if (type == "tsp")
				typeId = 2;

			if (params.size() > 4) {
				std::string specialRule = params[4];

				if (specialRule == "repeat")
				{
					//repeat move, no extra parameters
				}
				else if (specialRule == "jump")
				{
					//jump from one tile to another, extra parameters are target index followed by any number of points for the path to pass through
					int targetPosition = atoi(params[5].c_str());
					std::vector<glm::vec3> pathCoordinates;

					for (size_t i = 6; i < params.size(); i++)
					{
						std::vector<std::string> coords = split(params[i], ",");
						pathCoordinates.push_back(glm::vec3(atoi(coords[0].c_str()), atoi(coords[1].c_str()), atoi(coords[2].c_str())));
					}
				}
				else if (specialRule == "wait")
				{
					//wait a turn, extra parameters are the bounding coordinates (3d)
					std::vector<glm::vec3> boundCoordinates;

					for (size_t i = 5; i < 7; i++)
					{
						std::vector<std::string> coords = split(params[i], ",");
						boundCoordinates.push_back(glm::vec3(atoi(coords[0].c_str()), atoi(coords[1].c_str()), atoi(coords[2].c_str())));
					}
				}
				else if (specialRule == "lock")
				{
					//stay until saved by another player, extra parameter is the location to stay fixed at
					std::vector<std::string> coords = split(params[5], ",");
					glm::vec3 lockCoordinate(atoi(coords[0].c_str()), atoi(coords[1].c_str()), atoi(coords[2].c_str()));
				}

			}

			//TODO move this to include type and properties
			tileInfo.push_back(glm::vec4(atoi(position[0].c_str()), atoi(position[1].c_str()), atoi(rotation[0].c_str()), typeId));
		}
		else if (params[0] == "e") {
			std::string scale = params[3];
			std::string type = params[4];
			glm::vec3 rotationVector = stringVectorToVec3(rotation);
			rotationVector = glm::vec3(degToRad(rotationVector.x), degToRad(rotationVector.y), degToRad(rotationVector.z));
			if (params.size() > 4) {
				addEnvironmentObject(type, stringVectorToVec3(position), rotationVector, (float)atof(scale.c_str()));
			}
			else {
				addEnvironmentObject(type, stringVectorToVec3(position), rotationVector, 1.0f);
			}
		}
	}
	for (int i = 0; i < tileInfo.size(); i++)
	{
		addTile(i);// ,std::make_shared <Space>());
	}
}
void ObjectManager::addPlayer(int i) {
	/*std::shared_ptr<Player> player = std::make_shared<Player>(i,"groen", game);
	player->addComponent(std::make_shared<ModelComponent>("models/steve/steve.obj"));
	player->addComponent(std::make_shared<MoveToComponent>());
	objectList->push_back(player);*/
}
void ObjectManager::addTile(int tileNumber)//void ObjectManager::addTile(int tileNumber, std::shared_ptr <Space> space)
{
	addTile(tileNumber, 1.0f);
}
void ObjectManager::addTile(int tileNumber, float scale)//void ObjectManager::addTile(int tileNumber, std::shared_ptr <Space> space)
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
void ObjectManager::addEnvironmentObject(const std::string& fileName, glm::vec3 position, glm::vec3 rotation, float scale) {
	std::shared_ptr<GameObject> object = std::make_shared<GameObject>();
	object->addComponent(std::make_shared<ModelComponent>(fileName, scale));
	object->position = position;
	object->rotation = rotation;
	objectList->push_back(object);
}
