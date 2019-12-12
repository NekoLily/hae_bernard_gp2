#pragma once
#include <SFML/Graphics.hpp>
#include <math.h>
#include "Enum.h"
#include "Wall.h"
using namespace sf;

class	Shell
{
private:
	float		offSetSpawnPos = 33;
public:
	const char* shooterName;
	CircleShape	shell;
	float		xDirection = 0;
	float		yDirection = 0;
	float		offSetSpeed = 2.5f;
	bool		Explode = false;
	int			maxHit = 3;
	int			CurrentHit = 0;

	Shell(const char* _name, Vector2f _pos, float _size, Color _color = Color::Red)
	{
		shooterName = _name;
		shell.setRadius(_size);
		shell.setFillColor(_color);
		shell.setOrigin(Vector2f(shell.getRadius(), shell.getRadius()));
		shell.setPosition(_pos);
		
	}

	void	SetDirection(Vector2f _pos)
	{
		Vector2f	shellPos = shell.getPosition();
		float		xDistance = shellPos.x - _pos.x;
		float		yDistance = shellPos.y - _pos.y;
		float		Distance = sqrt(xDistance * xDistance + yDistance * yDistance);

		xDirection = -xDistance / Distance;
		yDirection = -yDistance / Distance;

		shell.setPosition(shellPos.x + xDirection * offSetSpawnPos, shellPos.y + yDirection * offSetSpawnPos);
	}

	void	CheckCollisionSide(Wall wall)
	{
		Rect<float> shellRect = shell.getGlobalBounds();
		Rect<float> wallRect = wall.wall.getGlobalBounds();

		float shellRectRight = shellRect.left + shellRect.width;
		float shellRectBot = shellRect.top + shellRect.height;
		float wallRectRight = wallRect.left + wallRect.width;
		float walllRectBot = wallRect.top + wallRect.height;

		//printf("shell L:%f R:%f T:%f B:%f\n", shellRect.left, shellRectRight, shellRect.top, shellRectBot);
		//printf("wall : %s L:%f R:%f  T:%f B:%f W:%f H:%f\n",wall.name,  wallRect.left, wallRectRight, wallRect.top, walllRectBot, wallRect.width, wallRect.height);

		if (shellRectRight > wallRect.left&& shellRect.left < wallRect.left && xDirection > 0)
		{
			//printf("Left\n");
			if (xDirection > 0)
				xDirection = -xDirection;
		}
		else if (shellRect.left <= wallRectRight && shellRectRight - 1 >= wallRectRight - 1 && xDirection < 0)
		{
			//printf("Right\n");
			if (xDirection < 0)
				xDirection = -xDirection;
		}
		else if (shellRectBot >= wallRect.top && shellRect.top <= wallRect.top)
		{
			//printf("UpSide\n");
			if (yDirection > 0)
				yDirection = -yDirection;
		}
		else if (shellRect.top <= walllRectBot && shellRectBot >= walllRectBot)
		{
			//printf("BotSide\n", yDirection);
			if (yDirection < 0)
				yDirection = -yDirection;
		}
		//printf("\n");
	}
};

