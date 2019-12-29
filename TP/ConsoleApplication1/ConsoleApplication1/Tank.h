#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <math.h>
#include "Enum.h"
#include "Wall.h"

using namespace sf;

class Tank : public Drawable, public Transformable
{
public:
	Transform		tankTransform;
	Transform		lastState;

	RectangleShape	rectangleTank;
	RectangleShape	gun;
	CircleShape		circleGun;

	const char*		name;

	int				currentShell = 0;
	int				maxShell = 3;
	float			lastShootingTime = 0;
	bool			IsAlive = true;
	float			tankSpeed = 2.f;

	Tank(const char* _name = "No name", Vector2f _pos = Vector2f(50, 50), Vector2f _size = Vector2f(30, 30), Color _color = Color::Red)
	{
		tankTransform = Transform::Identity;
		tankTransform.translate(_pos);
		name = _name;

		rectangleTank.setSize(_size);
		rectangleTank.setOrigin(Vector2f(_size.x / 2, _size.y / 2));
		rectangleTank.setFillColor(_color);
		rectangleTank.setOutlineColor(Color::Black);
		rectangleTank.setOutlineThickness(3);

		gun.setSize(Vector2f(_size.x + 10,_size.y /5));
		gun.setOrigin(Vector2f(0, gun.getSize().y / 2));
		gun.setFillColor(Color::Green);
		gun.setOutlineColor(Color::Black);
		gun.setOutlineThickness(1.5);

		circleGun.setFillColor(Color::Green);
		circleGun.setOutlineColor(Color::Black);
		circleGun.setOutlineThickness(2);
		circleGun.setRadius(10);
		circleGun.setOrigin(Vector2f(10, 10));

	};

	void	SetGunAngle(Vector2f mouseWorldPos)
	{
		float result = atan2f(mouseWorldPos.y - gun.getPosition().y, mouseWorldPos.x - gun.getPosition().x) * 180 / 3.14159265;
		gun.setRotation(result);
	}

	void	SetGunOnTank()
	{
		gun.setPosition(tankTransform.transformPoint(0, 5));
		circleGun.setPosition(tankTransform.transformPoint(0, 5));
	}

	void	MoveTank(MoveDirection _direction)
	{	
		switch (_direction)
		{
		case Up:
		{
			//rectangleTank.move(0, -tankSpeed);
			tankTransform.translate(0, -1);
			break;
		}
		case Left:
			//rectangleTank.move(-tankSpeed, 0);
			tankTransform.rotate(-1);
			break;
		case Right:
			//rectangleTank.move(tankSpeed, 0);
			tankTransform.rotate(1);
			break;
		case Down:
			//rectangleTank.move(0, tankSpeed);
			tankTransform.translate(0, 1);
			break;
		default:
			break;
		}
	}

	FloatRect	GetTankGlobalBounds()
	{
		return tankTransform.transformRect(rectangleTank.getGlobalBounds());
	}

	bool	CheckIfCollideWithWall(Wall wall)
	{
		if (GetTankGlobalBounds().intersects(wall.wall.getGlobalBounds()))
		{
			tankTransform = lastState;
			return true;
		}
		return false;
	}

	bool	CheckIfCollideWithOtherTank(Tank otherTank)
	{
		if (GetTankGlobalBounds().intersects(otherTank.GetTankGlobalBounds()))
		{
			tankTransform = lastState;
			return true;
		}
		return false;
	}

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= tankTransform;
		target.draw(rectangleTank, states);
		target.draw(gun);
		target.draw(circleGun);
	}
};
