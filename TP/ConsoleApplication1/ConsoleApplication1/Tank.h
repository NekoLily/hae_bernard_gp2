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
	const char* name;
	RectangleShape tank;
	RectangleShape gun;
	Vector2f lastposition;
	int CurrentShell = 0;
	int	MaxShell = 100;
	float lastShootingTime = 0;
	std::vector<Shell> shellList;
	bool IsAlive = true;
	Tank(const char* _name = "No name", Vector2f _POS = Vector2f(50, 50), Vector2f _Size = Vector2f(30, 30), Color _color = Color::Red)
	{
		name = _name;
		tank.setFillColor(_color);
		tank.setSize(_Size);
		tank.setOrigin(Vector2f(_Size.x / 2, _Size.y / 2));
		tank.setPosition(_POS);

		gun.setFillColor(Color::Green);
		gun.setSize(Vector2f(_Size.x / 1.5, _Size.y / 5));
		gun.setOrigin(Vector2f(0, gun.getSize().y / 2));
		gun.setPosition(_POS);
	};
	~Tank() {};

	void SetGunAngle(Vector2f mousePos)
	{
		float result = atan2f(mousePos.y - gun.getPosition().y, mousePos.x - gun.getPosition().x) * 180 / 3.14159265;
		gun.setRotation(result);
	}

	void SetGunToTankPosition()
	{
		gun.setPosition(tank.getPosition());
	}

	void Move(Direction _direction, float _speed)
	{
		Vector2f _pos = tank.getPosition();
		switch (_direction)
		{
		case Up:
			_pos.y -= _speed;
			break;
		case Left:
			_pos.x -= _speed;
			break;
		case Right:
			_pos.x += _speed;
			break;
		case Down:
			_pos.y += _speed;
			break;
		default:
			break;
		}
		tank.setPosition(_pos);
	}

	void	CreateShell(Vector2f _pos)
	{
		Shell shell(name, tank.getPosition(), 2.f);
		//shell.AddPoints(_pos);
		shell.Direction(_pos);
		shellList.push_back(shell);
	}

	void RemoveShell()
	{
		std::vector<Shell>::iterator it = shellList.begin();
		while (it != shellList.end())
		{
			if (it->currentRatio >= 1.0f || it->Explode)
				it = shellList.erase(it);
			else
				++it;
		}
		shellList.shrink_to_fit();
	}
};
