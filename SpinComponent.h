#pragma once
#include "Component.h"
#include <iostream>
class SpinComponent : public Component
{
	
public:

<<<<<<< HEAD
	float speed;
	SpinComponent(float speed);
=======
	int speed;
	SpinComponent(int speed);
>>>>>>> components
	SpinComponent();
	~SpinComponent();

	virtual void update(float elapsedTime) override;
};

