#include "MoveToComponent.h"
#include <iostream>


MoveToComponent::MoveToComponent(float startSpeed, glm::vec3 startPoint){
    speed = startSpeed;
    targetPoint = startPoint;
}
MoveToComponent::~MoveToComponent(){}

void MoveToComponent::update(float elapsedTime) {
    glm::vec3 direction = targetPoint - object->position;
    float distance = glm::length(direction);
    float movementDistance = speed * elapsedTime;

    if (movementDistance >= distance) {
        object->position = targetPoint;
        return;
    }

    glm::vec3 movement = direction * movementDistance;
    object->position -= movement;

	std::cout << object->position.x << "," << object->position.y << "," << object->position.z << std::endl;
	std::cout << direction.x << "," << direction.y << "," << direction.z << std::endl;
	float angle = atan2(object->position.z - targetPoint.z, object->position.x - targetPoint.x);

	object->rotation.y = 0.05f * angle + 0.95f * object->rotation.y;
}
