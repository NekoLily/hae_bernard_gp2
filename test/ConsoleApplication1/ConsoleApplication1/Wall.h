#pragma once
#include <SFML/Graphics.hpp>
#include "Enum.h"
using namespace sf;

class Wall
{
public:
	const char*		name;
	RectangleShape	wall;

	Wall(const char * _name, Vector2f _pos, Vector2f _size, Color _color )
	{
		name = _name;
		wall.setPosition(_pos);
		wall.setSize(_size);
		wall.setFillColor(Color(52, 45, 41, 255));
		wall.setOutlineColor(Color::Black);
		wall.setOutlineThickness(1);
	};
};

