#pragma once
#include<iostream>
#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Window.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>
#include"Bullet.h"
using namespace std;
using namespace sf;

class SpaceF
{
private:

	Sprite Spaceship;
	Vector2f velocity;

	int explosionframe;
	bool isExploding;
	Clock explodeClock;

	Bullet bullet;

	void initVariables();
public:
	SpaceF();
	virtual ~SpaceF();

	Sprite &getSpaceShip();
	//void setTexture(string path);

	void setExplosionFrame(int a);
	int getExplosionFrame();
	void setIsExploding(bool a);
	bool getIsExploding();
	Clock &getExplodeClock();

	Bullet &getBullet(Texture &texture,float velocity);
	void setVelocity(float x,float y);
	Vector2f getVelocity();
};

