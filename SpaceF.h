#pragma once
#include<iostream>
#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Window.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>
using namespace std;
using namespace sf;

class SpaceF
{
private:

	RectangleShape Spaceship;

	void initVariables();
public:
	SpaceF();
	virtual ~SpaceF();
	RectangleShape& getSpaceShip();
};

