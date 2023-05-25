#include "MoveToComponent.h"

MoveToComponent::MoveToComponent(){}
MoveToComponent::~MoveToComponent(){}

void MoveToComponent::update(float elapsedTime) {

	object->position = (1 - speed) * object->position + speed * targetPoint;

	float angle = atan2(object->position.z - targetPoint.z, object->position.x - targetPoint.x);

	object->rotation.y = 0.05f * angle + 0.95f * object->rotation.y;
}
