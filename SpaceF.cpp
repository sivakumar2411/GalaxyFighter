#include "SpaceF.h"


SpaceF::SpaceF() {
	this->initVariables();
}

SpaceF::~SpaceF() {

}

void SpaceF::initVariables() {
	this->Spaceship.setSize(Vector2f(100.f,100.f));
	//this->Spaceship.setScale(1.f,1.f);
	this->Spaceship.setFillColor(Color::Yellow);
	this->Spaceship.setOutlineColor(Color::Red);
	this->Spaceship.setOutlineThickness(1.f);
	//this->Spaceship.setPosition(10.f, 10.f);
}

RectangleShape& SpaceF::getSpaceShip() {
	return this->Spaceship;
}
