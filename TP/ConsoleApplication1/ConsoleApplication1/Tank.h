#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <math.h>
#include "Shell.h"
#include "Enum.h"

using namespace sf;
class Tank
{
public:
	const char*		name;
	RectangleShape	tank;
	RectangleShape	gun;
	CircleShape		circleGun;
	CircleShape		tankOrientation;
	Vector2f		lastposition;
	int				currentShell = 0;
	int				maxShell = 3;
	float			lastShootingTime = 0;
	bool			IsAlive = true;
	float			tankSpeed = 2.f;

	Tank(const char* _name = "No name", Vector2f _pos = Vector2f(50, 50), Vector2f _size = Vector2f(30, 30), Color _color = Color::Red)
	{
		name = _name;
		tank.setSize(_size);
		tank.setOrigin(Vector2f(_size.x / 2, _size.y / 2));
		tank.setPosition(_pos);
		tank.setFillColor(_color);
		tank.setOutlineColor(Color::Black);
		tank.setOutlineThickness(3);

		gun.setSize(Vector2f(_size.x, _size.y / 5));
		gun.setOrigin(Vector2f(0, gun.getSize().y / 2));
		gun.setFillColor(Color::Green);
		gun.setOutlineColor(Color::Black);
		gun.setOutlineThickness(1.5);

		circleGun.setFillColor(Color::Green);
		circleGun.setOutlineColor(Color::Black);
		circleGun.setOutlineThickness(2);
		circleGun.setRadius(10);
		circleGun.setOrigin(Vector2f(10, 10));

		tankOrientation.setRadius(3);
		tankOrientation.setOrigin(tankOrientation.getRadius(), tankOrientation.getRadius());
	};

	void	SetGunAngle(Vector2f mouseWorldPos)
	{
		float result = atan2f(mouseWorldPos.y - gun.getPosition().y, mouseWorldPos.x - gun.getPosition().x) * 180 / 3.14159265;
		gun.setRotation(result);
	}

	void	SetGunToTankPosition()
	{
		circleGun.setPosition(tank.getPosition());
		gun.setPosition(tank.getPosition());
	}

	void	MoveTank(MoveDirection _direction)
	{
		float		xDistance = tank.getPosition().x;
		float		yDistance = tank.getPosition().y;
		switch (_direction)
		{
		case Up:
		{
			tank.move(0, -tankSpeed);
			break;
		}
		case Left:
			tank.move(-tankSpeed, 0);
			break;
		case Right:
			tank.move(tankSpeed, 0);
			break;
		case Down:
			tank.move(0, tankSpeed);
			break;
		default:
			break;
		}
	}
};
