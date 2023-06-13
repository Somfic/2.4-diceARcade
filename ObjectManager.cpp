#include "ObjectManager.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

ObjectManager::ObjectManager(std::shared_ptr <std::list<std::shared_ptr<GameObject>>> objectList, std::string fileName, Game* game) //Game& game,
{
	this->game = game;
	this->objectList = objectList;
	initEnvironment(fileName);
}
ObjectManager::~ObjectManager() {

}
ObjectManager::ObjectManager() {

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
	std::shared_ptr<Space> newSpace = std::make_shared<NormalSpace>();
	newSpace->position = glm::vec3(0, 0, -2);
	newSpace->rotation = glm::vec3(0, 1.57, 0);
	newSpace->addComponent(std::make_shared<EmptyDrawComponent>());
	game->spaces->push_back(newSpace);
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
			std::shared_ptr<Space> space;
			int typeId;
			if (type == "str")
				typeId = 0;
			else if (type == "cor")
				typeId = 1;
			else if (type == "tsp")
				typeId = 2;

			space = std::make_shared<NormalSpace>();
			if (params.size() > 4) {
				std::string specialRule = params[4];

				if (specialRule == "repeat")
				{
					space = std::make_shared<GooseSpace>();
					//repeat move, no extra parameters
				}
				else if (specialRule == "jump")
				{
					
					//jump from one tile to another, extra parameters are target index followed by any number of points for the path to pass through
					int targetPosition = atoi(params[5].c_str());
					if (targetPosition == 12) {
						space = std::make_shared <BridgeSpace>();
					}
					else if (targetPosition == 37) {
						space = std::make_shared <MazeSpace>();
					}
					else if (targetPosition == 1) {
						space = std::make_shared <DeathSpace>();
					}
					std::vector<glm::vec3> pathCoordinates;

					for (size_t i = 6; i < params.size(); i++)
					{
						std::vector<std::string> coords = split(params[i], ",");
						pathCoordinates.push_back(glm::vec3(atoi(coords[0].c_str()), atoi(coords[1].c_str()), atoi(coords[2].c_str())));
					}
				}
				else if (specialRule == "wait")
				{
					space = std::make_shared<InnSpace>();
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
					space = std::make_shared<WaitSpace>();
					//stay until saved by another player, extra parameter is the location to stay fixed at
					std::vector<std::string> coords = split(params[5], ",");
					glm::vec3 lockCoordinate(atoi(coords[0].c_str()), atoi(coords[1].c_str()), atoi(coords[2].c_str()));
				}
			}
			space->position = glm::vec3(atof(position[0].c_str()),0, atof(position[1].c_str()));
			space->rotation = glm::vec3(0, (atof(rotation[0].c_str()) * 3.14f / 2),0);
			space->typeId = typeId;
			game->spaces->push_back(space);
			//TODO move this to include type and properties
			
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
	for (int i = 0; i < game->spaces->size(); i++)
	{
		addTile(i, game->spaces->at(i));
	}
	newSpace = std::make_shared<WinSpace>();
	newSpace->position = glm::vec3(18, 0, -18);
	newSpace->addComponent(std::make_shared<EmptyDrawComponent>());
	game->spaces->push_back(newSpace);
	for (int i = 0; i < 12; i++) {
		std::shared_ptr<Space> newSpace = std::make_shared<ExcessSpace>();
		newSpace->addComponent(std::make_shared<EmptyDrawComponent>());
		newSpace->position = glm::vec3(18, 0, -18);
		game->spaces->push_back(newSpace);
	}
}
void ObjectManager::addPlayer(std::shared_ptr<Player> player) {
	game->players.push_back(player);
	game->currentPlayer = player;
	player->position = glm::vec3(0,0,-2);
	if (player->color == "Red") {
		player->addComponent(std::make_shared<ModelComponent>("models/Tiles/goose.obj", 0.6, glm::vec4(1,0,0,1)));
	} else if(player->color == "Blue") {
		player->addComponent(std::make_shared<ModelComponent>("models/Tiles/goose.obj", 0.7, glm::vec4(0, 0, 1, 1)));
	}
	else if (player->color == "Green") {
		player->addComponent(std::make_shared<ModelComponent>("models/Tiles/goose.obj", 0.4, glm::vec4(0, 1, 0, 1)));
	}
	else if (player->color == "Yellow") {
		player->addComponent(std::make_shared<ModelComponent>("models/Tiles/goose.obj", 0.8, glm::vec4(1, 1, 0, 1)));
	}
	else if (player->color == "Purple") {
		player->addComponent(std::make_shared<ModelComponent>("models/Tiles/goose.obj", 0.9, glm::vec4(1, 0, 1, 1)));
	}
	else if (player->color == "Orange") {
		player->addComponent(std::make_shared<ModelComponent>("models/Tiles/goose.obj", 0.3, glm::vec4(1, 0.5, 0, 1)));
	}
	else {
		std::cout << "color selected doesn't exist" << std::endl;
		player->addComponent(std::make_shared<ModelComponent>("models/Tiles/goose.obj", 0.5));
	}

		player->addComponent(std::make_shared<MoveToComponent>(1.0f, player->position));
	player->addComponent(std::make_shared<PlayerMovmentComponent>(player,game));
	objectList->push_back(player);
}
void ObjectManager::addTile(int tileNumber, std::shared_ptr<Space>space)
{
	addTile(tileNumber, space, 1.0f);
}
void ObjectManager::addTile(int tileNumber, std::shared_ptr<Space>space, float scale)
{
	space->addComponent(std::make_shared<ModelComponent>("models/Tiles/tile.obj",scale));
	objectList->push_back(space);
	std::shared_ptr<GameObject> railing = std::make_shared<GameObject>();
	railing->position = space->position;
	railing->rotation = space->rotation;
	if (space->typeId == 1) {
		railing->addComponent(std::make_shared<ModelComponent>("models/Tiles/rail_curved.obj", scale));
	}
	else if (space->typeId == 2) {
		railing->addComponent(std::make_shared<ModelComponent>("models/Tiles/rail_straight.obj", scale));
	}
	else {
		railing->addComponent(std::make_shared<ModelComponent>("models/Tiles/rail_straight.obj", scale));
		std::shared_ptr<GameObject> railing2 = std::make_shared<GameObject>();
		railing2->addComponent(std::make_shared<ModelComponent>("models/Tiles/rail_straight.obj", scale));
		railing2->position = space->position;
		railing2->rotation = glm::vec3(0, space->rotation.y + 3.14, 0);
		objectList->push_back(railing2);

	}
	objectList->push_back(railing);
}
void ObjectManager::addEnvironmentObject(const std::string& fileName, glm::vec3 position, glm::vec3 rotation, float scale) {
	std::shared_ptr<GameObject> object = std::make_shared<GameObject>();
	object->addComponent(std::make_shared<ModelComponent>(fileName, scale));
	object->position = position;
	object->rotation = rotation;
	objectList->push_back(object);
}
