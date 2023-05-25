#pragma once
#include "Component.h"
#include <iostream>
class SpinComponent : public Component
{
	
public:
	float speed;
	SpinComponent(float speed);
	SpinComponent();
	~SpinComponent();

	virtual void update(float elapsedTime) override;
};

