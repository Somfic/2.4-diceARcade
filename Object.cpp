#include "Object.h"
#include "DrawComponent.h"
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>
//#include "BoundingBoxComponent.h"

Object::Object()
{
}

Object::~Object()
{
}

void Object::addComponent(std::shared_ptr<Component> component)
{
	component->setObject(this);
	components.push_back(component);

	if (drawComponent == nullptr)
		drawComponent = dynamic_pointer_cast<DrawComponent>(component);

}

std::list<std::shared_ptr<Component>> Object::getComponents()
{
	return components;
}


void Object::draw(const glm::mat4& parentMatrix)
{
	if (!drawComponent)
		return;

	glm::mat4 modelMatrix = parentMatrix;
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::rotate(modelMatrix, rotation.x, glm::vec3(1, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, rotation.z, glm::vec3(0, 0, 1));
	modelMatrix = glm::scale(modelMatrix, scale);

	tigl::shader->setModelMatrix(modelMatrix);
	drawComponent->draw();
}

void Object::update(float elapsedTime)
{
	for (auto& c : components)
		c->update(elapsedTime);
}