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
    float movementDistance = speed * elapsedTime + 0.1f;

    if (movementDistance >= distance) {
        object->position = targetPoint;
        return;
    }

    glm::vec3 movement = direction * movementDistance;
    object->position += movement;

	float angle = atan2(object->position.x - targetPoint.x, object->position.z - targetPoint.z);

    object->rotation.y = (angle);// *0.05f + 0.95f * object->rotation.y;
}
