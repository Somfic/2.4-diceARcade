#include "MoveToComponent.h"
#include <iostream>


MoveToComponent::MoveToComponent(float startSpeed, glm::vec3 startPoint){
    speed = startSpeed;
    targetPoint = startPoint;
}
MoveToComponent::~MoveToComponent(){}


glm::vec3 normalize(const glm::vec3& vector)
{
    float length = std::sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);

    if (length > 0.0f)
    {
        float invLength = 1.0f / length;
        return glm::vec3(vector.x * invLength, vector.y * invLength, vector.z * invLength);
    }
    else
    {
        return glm::vec3(0.0f, 0.0f, 0.0f);
    }
}

void MoveToComponent::update(float elapsedTime) {
    glm::vec3 direction = targetPoint - object->position;
    float distance = glm::length(direction);
    float movementDistance = speed * elapsedTime + 0.1;
    direction = normalize(direction);
    if (movementDistance >= distance) {
        object->position = targetPoint;
        return;
    }

    glm::vec3 movement = direction * movementDistance;
    object->position += movement;

	float angle = atan2(object->position.x - targetPoint.x, object->position.z - targetPoint.z);

    object->rotation.y = (angle);// *0.05f + 0.95f * object->rotation.y;
}

