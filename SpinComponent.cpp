#include "SpinComponent.h"


SpinComponent::SpinComponent(float speed) {
	this->speed = speed;
}
SpinComponent::SpinComponent() {
	this->speed = 0;
}
SpinComponent::~SpinComponent() {

}
void SpinComponent::update(float elapsedTime) {
	object->rotation.y += speed * elapsedTime;
}