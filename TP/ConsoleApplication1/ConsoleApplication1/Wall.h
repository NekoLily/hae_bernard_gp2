#pragma once
#include <SFML/Graphics.hpp>
#include "Enum.h"

using namespace sf;

class Wall
{
private:
	Color wallColor = Color(52, 45, 41, 255);

public:
	const char*		name;
	RectangleShape	wall;

	Wall(const char * _name, Vector2f _pos, Vector2f _size)
	{
		name = _name;
		wall.setPosition(_pos);
		wall.setSize(_size);
		wall.setFillColor(wallColor);
		wall.setOutlineColor(wallColor);
		wall.setOutlineThickness(1);
	};
};

