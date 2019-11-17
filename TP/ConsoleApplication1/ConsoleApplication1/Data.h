#pragma once
#include <list>
#include "Tank.h"
#include "Wall.h"
#include "Shell.h"
#include "Enum.h"


class Data
{
public:
	std::vector<Tank> tankList;
	std::vector<Wall> wallList;


	void	AddTank(const char* _name, Vector2f _position, Vector2f _size, Color _color = Color::Red)
	{
		Tank tank = Tank(_name, _position, _size, _color);
		tankList.push_back(tank);
	}

	void	RemoveTank()
	{
		std::vector<Tank>::iterator it = tankList.begin();
		while (it != tankList.end())
		{
			if (it->IsAlive == false)
				it = tankList.erase(it);
			else
				++it;
		}
		tankList.shrink_to_fit();
	}

	void	AddWall(const char* _name,Axe _axe,  Vector2f _position, Vector2f _size, Color _color = Color::White)
	{
		Wall wall = Wall(_name, _axe, _position, _size, _color);
		wallList.push_back(wall);
	}
};

