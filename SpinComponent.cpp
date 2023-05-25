#include "SpinComponent.h"


SpinComponent::SpinComponent(int speed) {
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