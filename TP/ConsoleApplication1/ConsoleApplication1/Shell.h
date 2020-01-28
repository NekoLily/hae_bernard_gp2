#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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
	float					offSetSpawnPos = 30;
	int						currentExplosion = 0;
	float					lastExplosionTime = 0;
	float					xDirection = 0;
	float					yDirection = 0;
	float					offSetSpeed = 5;
	int						maxHit = 3;
	int						currenthit = 0;
	Sound					shellSound;
	vector<Texture*>		explosionTexture;
	vector<SoundBuffer*>	soundBufferVec;

public:
	Tank*		owner = nullptr;
	Sprite		shell;
	ShellState	shellState = ShellState::Shellinit;
	
	Shell(Tank* _ownerTank, Vector2f _targetDirection, Texture* _shellTexture, vector<Texture*> _explosionTexture, vector<SoundBuffer*> _soundBufferVec)
	{
		owner = _ownerTank;
		shell.setTexture(*_shellTexture);
		shell.setOrigin(_shellTexture->getSize().x / 2, _shellTexture->getSize().y / 2);
		shell.setPosition(owner->GetTransformPosition());
		SetDirection(_targetDirection);
		explosionTexture = _explosionTexture;
		soundBufferVec = _soundBufferVec;	
		
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
	}

	bool	CheckIfCollideWithTank(Tank& tank)
	{
		if (shell.getGlobalBounds().intersects(tank.GetTankGlobalBounds()))
		{
			if (&tank == owner && shellState == ShellState::Shellinit)
			{
				shellSound.setBuffer(*soundBufferVec[3]);
				shellSound.play();
				printf("hit\n");
				return false;
			}
			if (tank.tankState == TankState::TankAlive)
			{
				tank.tankState = TankState::TankExplode;
				printf("%s has destroy !\n", tank.name);
				shellState = ShellState::ShellExplode;
				owner->currentShell--;
				return true;
			}	
		}
		return false;
	}

	bool	CheckIfCollideWithWall(Wall& wall)
	{
		if (shell.getGlobalBounds().intersects(wall.wall.getGlobalBounds()))
		{
			if (shellState == ShellState::Shellinit)
				shellState = ShellState::ShellMoving;
			currenthit++;
			if (currenthit >= maxHit)
			{
				shellState = ShellState::ShellExplode;
				owner->currentShell--;
			}
			else
				CheckCollisionSide(wall);
			return true;
		}
		return false;
	}

	bool	CheckIfCollideWithOtherShell(Shell& otherShell)
	{
		if (this == &otherShell)
			return false;
		else if (otherShell.shellState != ShellState::ShellExplode && shell.getGlobalBounds().intersects(otherShell.shell.getGlobalBounds()))
		{
			shellState = ShellState::ShellExplode;
			otherShell.shellState = ShellState::ShellExplode;
			otherShell.owner->currentShell--;
			owner->currentShell--;
			return true;
		}
		return false;
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
		if (shellState == ShellState::ShellMoving || shellState == ShellState::Shellinit)
			shell.move(xDirection * offSetSpeed, yDirection * offSetSpeed);
	}

	void	DoExplosion(float Currenttime)
	{
		if ((Currenttime - lastExplosionTime) > 0.05f || lastExplosionTime == 0)
		{
			if (currentExplosion == 0)
			{
				shellSound.setBuffer(*soundBufferVec[1]);
				shellSound.play();
				shell.setScale(0.5f, 0.5f);
			}
				
			if (currentExplosion < 9)
			{
				shell.setTexture(*explosionTexture[currentExplosion], true);
				shell.setOrigin(shell.getTexture()->getSize().x / 2, shell.getTexture()->getSize().y / 2);
				lastExplosionTime = Currenttime;
				currentExplosion++;
			}
			else if (currentExplosion == 9)
				shell.setTextureRect(IntRect(0,0,0,0));
			else if ((Currenttime - lastExplosionTime) > 0.02f)
			{
				delete(&shellSound);
				shellState = ShellState::ShellDestroy;
			}
				
		}
	}
};

