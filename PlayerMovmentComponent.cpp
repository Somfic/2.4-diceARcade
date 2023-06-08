#include "PlayerMovmentComponent.h"
PlayerMovmentComponent::PlayerMovmentComponent(std::shared_ptr<Player> players) {
	player = players;
	currentStandingSpace = std::make_shared<Space>();
}
PlayerMovmentComponent::~PlayerMovmentComponent() {
}
void PlayerMovmentComponent::update(float elapsedTime) {
	if (player->getCurrentSpace()->position != currentStandingSpace->position) {
		player->getCurrentSpaceIndex();

		currentStandingSpace = player->getCurrentSpace();
	}
	player->getComponent<MoveToComponent>()->targetPoint = currentStandingSpace->position;
}