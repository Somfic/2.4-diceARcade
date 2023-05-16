#pragma once

class Object;

class Component
{
protected:
	Object* object;
public:
	Component();
	~Component();

	virtual void update(float elapsedTime) {};

	inline void setObject(Object* object) { this->object = object; }
};

