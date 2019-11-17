#pragma once
#include <SFML/Graphics.hpp>
#include <math.h>
using namespace sf;

class Shell
{
	public:
		const char* shooterName;
		CircleShape	shell;
		std::vector<Vector2f> points;
		float currentRatio = 0;
		float xVelocity = 0;
		float yVelocity = 0;
		bool Explode = false;
		Shell(const char* _name, Vector2f _pos, float _size, Color _color = Color::Red)
		{
			shooterName = _name;
			shell.setRadius(_size);
			shell.setFillColor(_color);
			shell.setPosition(_pos);
			AddPoints(_pos);
		}

		void	AddPoints(Vector2f _pos)
		{
			points.push_back(_pos);
		}

		void	Direction(Vector2f _pos)
		{
			Vector2f BallPos = shell.getPosition();
			float Distance = sqrt((BallPos.x - _pos.x) * (BallPos.x - _pos.x) + (BallPos.y - _pos.y) * (BallPos.y - _pos.y));
			printf("Distance = %f\n", Distance);

			xVelocity = -(BallPos.x - _pos.x) / Distance;
			yVelocity = -(BallPos.y -_pos.y) /Distance;
		}
};

