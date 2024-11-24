
#include "Enemy.h"

Enemy::Enemy() {
	this->shootCooldown = 0;
}


Enemy::~Enemy() {

}

void Enemy::setShootCoolDown(float a) {
	this->shootCooldown = 1000.f;
}

float Enemy::getShootCoolDown() {
	return this->shootCooldown;
}