#pragma once
#include <SFML/Graphics.hpp>
#include <math.h>
using namespace sf;

class	Shell
{
private:
	float		offSetSpawnPos = 30;
public:
	const char* shooterName;
	CircleShape	shell;
	float		xDirection = 0;
	float		yDirection = 0;
	float		offSetSpeed = 5;
	bool		Explode = false;
	int			maxHit = 2;
	int			CurrentHit = 0;

	Shell(const char* _name, Vector2f _pos, float _size, Color _color = Color::Red)
	{
		shooterName = _name;
		shell.setRadius(_size);
		shell.setFillColor(_color);
		shell.setPosition(_pos);
	}

	void	Direction(Vector2f _pos)
	{
		Vector2f	shellPos = shell.getPosition();
		float		xDistance = shellPos.x - _pos.x;
		float		yDistance = shellPos.y - _pos.y;
		float		Distance = sqrt(xDistance * xDistance + yDistance * yDistance);

		xDirection = -xDistance / Distance;
		yDirection = -yDistance / Distance;

		shell.setPosition(shellPos.x + xDirection * offSetSpawnPos, shellPos.y + yDirection * offSetSpawnPos);
	}
};

