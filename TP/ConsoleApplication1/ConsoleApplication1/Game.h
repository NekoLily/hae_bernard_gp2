#pragma once
#include <list>
#include <vector>
#include "UI.h"
#include "Tank.h"
#include "Wall.h"
#include "Shell.h"
#include "Enum.h"


class Game
{
public:
	std::vector<Tank>			tankList;
	std::vector<Wall>			wallList;
	std::vector<Shell>			shellList;
	std::vector<Button>			ButtonList;
	std::vector<CircleShape>	mouseEffectList;

	void	AddTank(const char* _name, Vector2f _position, Color _color, Shader* _shader, Texture* _hull, Texture* _gun, vector<Texture*> explosionTexture)
	{
		tankList.push_back(Tank(_name, _position, _color, _shader, _hull, _gun, explosionTexture));
	}

	void	AddWall(const char* _name, Vector2f _position, Vector2f _size, Color _color = Color::White)
	{
		Wall wall = Wall(_name, _position, _size, _color);
		wallList.push_back(wall);
	}

	void	AddShell(Tank& _tank, Vector2f _position, Texture* shellTexture, vector<Texture*> explosionTexture)
	{
		Shell			shell = Shell(_tank.name, _tank.tankTransform.transformPoint(0, 0), shellTexture, explosionTexture);
		shell.owner = &_tank;
		shell.SetDirection(_position);
		shellList.push_back(shell);
	}

	void	ClearData()
	{
		tankList.clear();
		wallList.clear();
		shellList.clear();
	}

	void	TankManager(float time)
	{
		std::vector<Tank>::iterator it = tankList.begin();
		while (it != tankList.end())
		{
			if (it->name != "Player")
			{
				//tank.SetGunAngle(_data.tankList[0].rectangleTank.getPosition());
				//AddShell(tank, _data.tankList[0].rectangleTank.getPosition(), time);
			}
			for (Tank& otherTank : tankList)
			{
				if (it->name != otherTank.name && it->CheckIfCollideWithOtherTank(otherTank))
					break;
			}
			if (it->tankState == TankState::TankExplode)
				it->DoExplosion(time);
			if (it->tankState == TankState::Die)
			{
				printf("Tank %s destroyed \n", it->name);
				it = tankList.erase(it);
			}
			else
				++it;
		}
		tankList.shrink_to_fit();


		for (Tank& tank : tankList)
		{
			if (tank.name != "Player")
			{
				//tank.SetGunAngle(_data.tankList[0].rectangleTank.getPosition());
				//AddShell(tank, _data.tankList[0].rectangleTank.getPosition(), time);
			}
			for (Tank& otherTank : tankList)
			{
				if (tank.name != otherTank.name && tank.CheckIfCollideWithOtherTank(otherTank))
					break;
			}
			for (Wall& wall : wallList)
			{
				if (tank.CheckIfCollideWithWall(wall))
					break;
			}
			tank.lastTankTransform = tank.tankTransform;
		}
	}

	void	ShellManager(float time)
	{
		std::vector<Shell>::iterator it = shellList.begin();
		while (it != shellList.end())
		{

			if (it->shellState == ShellState::Moving)
			{
				for (Tank& tank : tankList)
				{
					if (it->CheckIfCollideWithTank(tank))
						break;
				}
				for (Wall& wall : wallList)
				{
					it->CheckIfCollideWithWall(wall);
					if (it->shellState == ShellState::ShellExplode)
						break;
				}
				it->MoveShell();
			}
			if (it->shellState == ShellState::ShellExplode)
				it->DoExplosion(time);
			if (it->shellState == ShellState::Destroy)
				it = shellList.erase(it);
			else
				++it;
		}
		shellList.shrink_to_fit();
	}
};

