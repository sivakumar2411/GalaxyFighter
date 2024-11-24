#include "SpaceF.h"


SpaceF::SpaceF() {
	this->initVariables();
}

SpaceF::~SpaceF() {

}

void SpaceF::initVariables() {
	//this->Spaceship.setScale(1.f,1.f);
	//this->Spaceship.setPosition(10.f, 10.f);
}

void SpaceF::setVelocity(float x, float y) {
	this->velocity = Vector2f(x, y);
}

//void SpaceF::setTexture(string path) {
//	if (!this->SpaceTexture.loadFromFile(path)) {
//		cout << "Error at SetTexture On SpaceShip Class.\n";
//	}
//	Vector2u Scale = this->SpaceTexture.getSize();
//	this->Spaceship.setTexture(this->SpaceTexture);
//	this->Spaceship.setScale(50.f/ static_cast<float>(Scale.x),50.f/ static_cast<float>(Scale.y));
//}

Bullet &SpaceF::getBullet(Texture &texture,float a) {
	this->bullet = Bullet(a);
	this->bullet.getBullet().setTexture(texture);
	Vector2u Scale = texture.getSize();
	this->bullet.getBullet().setScale(50.f / static_cast<float>(Scale.x), 50.f / static_cast<float>(Scale.y));
	return this->bullet;
}

Vector2f SpaceF::getVelocity() {
	return this->velocity;
}

void SpaceF::setExplosionFrame(int a) {
	this->explosionframe = a;
}

int SpaceF::getExplosionFrame() {
	return this->explosionframe;
}

void SpaceF::setIsExploding(bool a) {
	this->isExploding = a;
}

bool SpaceF::getIsExploding() {
	return this->isExploding;
}

Clock &SpaceF::getExplodeClock() {
	return this->explodeClock;
}

Sprite& SpaceF::getSpaceShip() {
	return this->Spaceship;
}
