#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <math.h>
#include "Enum.h"
#include "Wall.h"
#include "Tank.h"

using namespace sf;
using namespace std;

class	Shell
{
private:
	float		offSetSpawnPos = 65;
	int			currentExplosion = 0;
	float		lastExplosionTime = 0;
	float		xDirection = 0;
	float		yDirection = 0;
	float		offSetSpeed = 5;
	int			maxHit = 3;
	int			currenthit = 0;

public:
	Tank* owner = nullptr;
	Sprite	shell;
	ShellState	shellState = ShellState::Create;
	vector<Texture*> explosionTexture;

	Shell(const char* _name, Vector2f _pos, Texture* _shellTexture, vector<Texture*> _explosionTexture)
	{
		shell.setTexture(*_shellTexture);

		shell.setOrigin(_shellTexture->getSize().x / 2, _shellTexture->getSize().y / 2);
		shell.setPosition(_pos);
		explosionTexture = _explosionTexture;
	}

	void	SetDirection(Vector2f _pos)
	{
		Vector2f	shellPos = shell.getPosition();
		float		xDistance = shellPos.x - _pos.x;
		float		yDistance = shellPos.y - _pos.y;
		float		Distance = sqrt(xDistance * xDistance + yDistance * yDistance);

		xDirection = -xDistance / Distance;
		yDirection = -yDistance / Distance;

		float result = atan2f((shellPos.y + yDirection) - shellPos.y, (shellPos.x + xDirection) - shellPos.x) * 180 / 3.14159265;
		shell.setRotation(result + 90);
		shell.setPosition(shellPos.x + xDirection * offSetSpawnPos, shellPos.y + yDirection * offSetSpawnPos);
		shellState = ShellState::Moving;
	}

	bool	CheckIfCollideWithTank(Tank& tank)
	{
		if (shell.getGlobalBounds().intersects(tank.GetTankGlobalBounds()))
		{
			//tank.IsAlive = false;
			//tank.tankState = TankState::TankExplode;
			shellState = ShellState::ShellExplode;
			owner->currentShell--;
			return true;
		}
		return false;
	}

	void	CheckIfCollideWithWall(Wall& wall)
	{
		if (shell.getGlobalBounds().intersects(wall.wall.getGlobalBounds()))
		{
			currenthit++;
			if (currenthit >= maxHit)
			{
				shellState = ShellState::ShellExplode;
				owner->currentShell--;
			}
			else
				CheckCollisionSide(wall);
		}
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
		Vector2f	shellPos = shell.getPosition();
		float result = atan2f((shellPos.y + yDirection) - shellPos.y, (shellPos.x + xDirection) - shellPos.x) * 180 / 3.14159265;
		shell.setRotation(result + 90);
		//printf("\n");
	}

	void	MoveShell()
	{
		if (shellState == ShellState::Moving)
			shell.move(xDirection * offSetSpeed, yDirection * offSetSpeed);
	}

	void	DoExplosion(float Currenttime)
	{
		if ((Currenttime - lastExplosionTime) > 0.05f || lastExplosionTime == 0)
		{	
			if (currentExplosion == 0)
				shell.setScale(0.5f, 0.5f);
			if (currentExplosion < 8)
			{
				shell.setTexture(*explosionTexture[currentExplosion], true);
				shell.setOrigin(shell.getTexture()->getSize().x / 2, shell.getTexture()->getSize().y / 2);
				lastExplosionTime = Currenttime;
				currentExplosion++;
			}
			else
				shellState = ShellState::Destroy;
		}
	}
};

