#pragma once
#include "Component.h"
class MoveToComponent :public Component{
public:
	float speed = 0;
	glm::vec3 targetPoint;

	MoveToComponent(float speed, glm::vec3 targetPoint);
	~MoveToComponent();

	void update(float elapsedTime) override;

};

