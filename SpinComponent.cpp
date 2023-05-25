#include "SpinComponent.h"


<<<<<<< HEAD
SpinComponent::SpinComponent(float speed) {
=======
SpinComponent::SpinComponent(int speed) {
>>>>>>> components
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