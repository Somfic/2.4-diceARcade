#pragma once
#include "Component.h"
class MoveToComponent :public Component{
public:
	float speed = 0;
	glm::vec3 targetPoint;

	MoveToComponent();
	~MoveToComponent();

	void update(float elapsedTime) override;

};

