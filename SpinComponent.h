#pragma once
#include "Component.h"
#include <iostream>
class SpinComponent : public Component
{
	
public:

	int speed;
	SpinComponent(int speed);
	SpinComponent();
	~SpinComponent();

	virtual void update(float elapsedTime) override;
};

