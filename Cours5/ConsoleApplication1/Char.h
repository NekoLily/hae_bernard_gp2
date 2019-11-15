#pragma once
#include <SFML/Graphics.hpp>
#include <string>

using namespace sf;
class Char 
{
public:
	Vector2f position;
	Sprite sprite;
	RectangleShape tank;
	const char * name;
	Char(Vector2f _POS = Vector2f(50, 50), Vector2f _Size = Vector2f(30, 30), const char *_name = "No name", Color _color = Color::Red)
	{
		name = _name;
		tank.setFillColor(_color);
		tank.setSize(_Size);
		position  = _POS;
		tank.setPosition(_POS);
	};
	~Char(){};

	void SetPosition()
	{
		tank.setPosition(position);
	}
};

