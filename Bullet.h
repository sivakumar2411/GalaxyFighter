#pragma once
#include<iostream>
#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Window.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>
using namespace std;
using namespace sf;

class Bullet
{
private:
	Sprite bullet;
	int bulletType;
	float velocity;
public:
	Bullet();
	Bullet(float a);
	virtual ~Bullet();

	Sprite &getBullet();
	void setBulletType(int a);
	int getBulletType();
	void setBulletVelocity(float a);
	float getBulletVelocity();
};

