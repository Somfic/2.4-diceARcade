#include "PlayerMovmentComponent.h"
PlayerMovmentComponent::PlayerMovmentComponent(std::shared_ptr<Player> players, Game* game) {
	this->game = game;
	player = players;
	currentStandingSpace = 1;	
}
PlayerMovmentComponent::~PlayerMovmentComponent() {
}
void PlayerMovmentComponent::update(float elapsedTime) {

	if (player->position == game->getSpaces()[currentStandingSpace]->position) {
		if (player->getCurrentSpaceIndex() > currentStandingSpace){
			currentStandingSpace++;
		}
		else if (player->getCurrentSpaceIndex() < currentStandingSpace) {
			currentStandingSpace--;
		}
		
	}
	std::cout << game->getSpaces()[currentStandingSpace]->position.x << "." << game->getSpaces()[currentStandingSpace]->position.y << "." << game->getSpaces()[currentStandingSpace]->position.z << std::endl;

	player->getComponent<MoveToComponent>()->targetPoint = game->getSpaces()[currentStandingSpace]->position;
	
}