#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;

class Flash
{
private:
	
	float					offSetSpawnPos = 75;
	int						currentFlash = 0;
	float					lastFlashTime = 0;
	float					xDirection = 0;
	float					yDirection = 0;
	vector<Texture*>		flashTextureVec;
public:
	
	FlashState	flashState = FlashState::FlashPlaying;
	Sprite		flash;

	Flash(Tank* _owner, Vector2f _targetDirection, vector<Texture*> _flashTexture)
	{
		flash.setTexture(*_flashTexture[0]);
		flash.setPosition(_owner->tankTransform.transformPoint(0, 0));
		flash.setOrigin(_flashTexture[0]->getSize().x / 2, _flashTexture[0]->getSize().y / 2);
		SetDirection(_targetDirection);
		flashTextureVec = _flashTexture;
	}

	void	SetDirection(Vector2f _pos)
	{
		Vector2f	flashPos = flash.getPosition();
		float		xDistance = flashPos.x - _pos.x;
		float		yDistance = flashPos.y - _pos.y;
		float		Distance = sqrt(xDistance * xDistance + yDistance * yDistance);

		xDirection = -xDistance / Distance;
		yDirection = -yDistance / Distance;

		float result = atan2f((flashPos.y + yDirection) - flashPos.y, (flashPos.x + xDirection) - flashPos.x) * 180 / 3.14159265;
		flash.setRotation(result + 90);
		flash.setPosition(flashPos.x + xDirection * offSetSpawnPos, flashPos.y + yDirection * offSetSpawnPos);
	}

	void	DoFlash(float Currenttime)
	{
		if ((Currenttime - lastFlashTime) > 0.02f || lastFlashTime == 0)
		{
			if (currentFlash == 0)
			{
				//shellSound.setBuffer(*soundBufferVec[1]);
				//shellSound.play();
				//shell.setScale(0.5f, 0.5f);
			}

			if (currentFlash < 4)
			{
				flash.setTexture(*flashTextureVec[currentFlash], true);
				flash.setOrigin(flash.getTexture()->getSize().x / 2, flash.getTexture()->getSize().y / 2);
				lastFlashTime = Currenttime;
				currentFlash++;
			}
			else if ((Currenttime - lastFlashTime) > 0.5f)
				flashState = FlashState::FlashFisnih;
		}
	}

};