#pragma once
#include <list>
#include <vector>
#include <SFML/Audio.hpp>
#include "Tank.h"
#include "Wall.h"
#include "Shell.h"
#include "Flash.h";
#include "Enum.h"


class Game
{
private:
	int							playerAliveNumber = 0;
	int							botAliveNumber = 0;
	int							indexTexture = 0;


	
public:
	std::vector<Tank>			tankList;
	std::vector<Wall>			wallList;
	std::vector<Shell>			shellList;
	std::vector<Flash>			flashList;

	std::vector<CircleShape>	mouseEffectList;

	Texture*					shellTexture;
	vector<Texture*>			tankTexture;
	vector<Texture*>			explosionTexture;
	vector<Texture*>			flashTexture;
	vector<SoundBuffer*>		soundBufferVec;

	GameMode					gameMode;

	std::string					nameVersusWinner;

	void		DrawElement(RenderWindow& win)
	{
		for (Wall& wall : wallList)
			win.draw(wall.wall);
		for (Tank& tank : tankList)
			if (tank.tankState != TankState::TankDead)
				win.draw(tank);
		for (Shell& shell : shellList)
			win.draw(shell.shell);
		for (Flash& flash : flashList)
			win.draw(flash.flash);
	}

	void		AddTank(const char* _name, TankTag _tankTag, Vector2f _position,  float _startRotation)
	{
		Texture* hull = nullptr;
		Texture* gun = nullptr;

		
		switch (_tankTag)
		{
		case PlayerTank:
			hull = tankTexture[playerAliveNumber + indexTexture];
			gun = tankTexture[++playerAliveNumber + indexTexture++];
			break;
		case BotTank:
			hull = tankTexture[2];
			gun = tankTexture[3];
			botAliveNumber++;
			break;
		default:
			break;
		}
		tankList.push_back(Tank(_name, _tankTag, _position, hull, gun, _startRotation, explosionTexture, soundBufferVec));
	}

	void		AddWall(const char* _name, Vector2f _position, Vector2f _size)
	{
		wallList.push_back(Wall(_name, _position, _size));
	}

	void		AddShell(Tank& _tank, Vector2f _targetDirection, float _time)
	{
		if (_tank.currentShell < _tank.maxShell && ((_time - _tank.lastShootingTime) > 0.2f || _tank.lastShootingTime == 0))
		{
			_tank.lastShootingTime = _time;
			_tank.currentShell++;
			shellList.push_back(Shell(&_tank, _targetDirection, shellTexture, explosionTexture, soundBufferVec));
			flashList.push_back(Flash(&_tank, _targetDirection, flashTexture));
		}
	}

	void		ResetData()
	{
		tankList.clear();
		wallList.clear();
		shellList.clear();
		playerAliveNumber = 0;
		botAliveNumber = 0;
		indexTexture = 0;
	}

	void		TankManager(float time)
	{
		std::vector<Tank>::iterator it = tankList.begin();
		while (it != tankList.end())
		{
			if (it->tankState == TankState::TankAlive)
			{
				if (it->tankTag == TankTag::BotTank)
				{
					//tank.SetGunAngle(_data.tankList[0].rectangleTank.getPosition());
					//AddShell(tank, _data.tankList[0].rectangleTank.getPosition(), time);
				}
				for (Tank& otherTank : tankList)
					if (it->CheckIfCollideWithOtherTank(otherTank))
						break;
				for (Wall& wall : wallList)
					if (it->CheckIfCollideWithWall(wall))
						break;
				it->lastTankTransform = it->tankTransform;
			}
			if (it->tankState == TankState::TankExplode)
				it->DoExplosion(time);
			if (it->tankState == TankState::TankDestroy)
			{
				switch (it->tankTag)
				{
				case PlayerTank:
					playerAliveNumber--;
					break;
				case BotTank:
					botAliveNumber--;
					break;
				default:
					break;
				}
				it->tankState = TankState::TankDead;
				break;
			}
			++it;
		}
	}

	void		ShellManager(float time)
	{
		std::vector<Shell>::iterator it = shellList.begin();
		while (it != shellList.end())
		{
			if (it->shellState == ShellState::ShellMoving || it->shellState == ShellState::Shellinit)
			{
				it->MoveShell();
				for (Tank& tank : tankList)
					if (it->CheckIfCollideWithTank(tank))
						break;
				for (Wall& wall : wallList)
					if (it->CheckIfCollideWithWall(wall))
						break;
				for (Shell& otherShell : shellList)
					if (it->CheckIfCollideWithOtherShell(otherShell))
						break;
			}
			if (it->shellState == ShellState::ShellExplode)
				it->DoExplosion(time);
			if (it->shellState == ShellState::ShellDestroy)
				it = shellList.erase(it);
			else
				++it;
		}
		shellList.shrink_to_fit();
	}

	void		FlashManager(float time)
	{
		std::vector<Flash>::iterator it = flashList.begin();
		while (it != flashList.end())
		{
			if (it->flashState == FlashState::FlashPlaying)
				it->DoFlash(time);
			if (it->flashState == FlashState::FlashFisnih)
				it = flashList.erase(it);
			else
				++it;
		}
		flashList.shrink_to_fit();
	}

	InGameState	GameManager()
	{
		if (gameMode == GameMode::Solo)
		{
			if (playerAliveNumber == 0)
				return InGameState::PlayerLoose;
			else if (botAliveNumber == 0)
				return InGameState::PlayerWin;
			else
				return InGameState::Playing;
		}
		else if (gameMode == GameMode::Versus)
		{
			if (playerAliveNumber == 0)
				return InGameState::Draw;
			else if (playerAliveNumber == 1)
			{
				for (Tank& tank : tankList)
				{
					if (tank.tankState == TankState::TankAlive)
					{
						nameVersusWinner = tank.name;
						return InGameState::VersusEnd;
					}
				}
			}
			else
				return InGameState::Playing;
		}
	}
};

