#include "Bullet.h"


Bullet::Bullet() {

}

Bullet::Bullet(float velocity) {
	this->bullet.setOrigin(0.f, 0.f);
	this->velocity = velocity;
	this->bulletType = 1;
}

Bullet::~Bullet() {

}

void Bullet::setBulletType(int a) {
	this->bulletType = a;
}

int Bullet::getBulletType() {
	return this->bulletType;
}

void Bullet::setBulletVelocity(float a) {
	this->velocity = a;
}

float Bullet::getBulletVelocity() {
	return this->velocity;
}

Sprite &Bullet::getBullet() {
	return this->bullet;
}