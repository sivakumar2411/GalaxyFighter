#pragma once
#include "SpaceF.h"
#include<iostream>

using namespace std;
using namespace sf;

class Enemy : public SpaceF
{
private:
	Texture Enemytexture;
	float shootCooldown;
public:
	Enemy();
	virtual ~Enemy();

	void setShootCoolDown(float a);
	float getShootCoolDown();
};

