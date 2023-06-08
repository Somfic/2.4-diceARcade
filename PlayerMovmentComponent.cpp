#include "PlayerMovmentComponent.h"
#include <iostream>
PlayerMovmentComponent::PlayerMovmentComponent(std::shared_ptr<Player> players, Game* game) {
	this->game = game;
	player = players;
	currentStandingSpace = 0;	
}
PlayerMovmentComponent::~PlayerMovmentComponent() {
}
void PlayerMovmentComponent::update(float elapsedTime) {

	if (player->position == game->getSpaces()[currentStandingSpace]->position) {
		if (player->getCurrentSpaceIndex() > currentStandingSpace){
			currentStandingSpace++;
			isFinished = false;
		}
		else if (player->getCurrentSpaceIndex() < currentStandingSpace) {
			currentStandingSpace--;
			isFinished = false;
		}
		else {
			isFinished = true;
		}
		
	}
	player->getComponent<MoveToComponent>()->targetPoint = game->getSpaces()[currentStandingSpace]->position;
	
}