#pragma once
#include <string>
#include <math.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Enum.h"
#include "Wall.h"

using namespace std;
using namespace sf;

class Tank : public Drawable, public Transformable
{
private:
	Shader* shaderptr;

	vector<Texture*>		explosionTexture;
	vector<SoundBuffer*>	soundBufferVec;
	Sound					tankSound;
	int						currentExplosion = 0;
	float					lastExplosionTime = 0;
	float					tankSpeed = 2;
	MoveDirection			lastState;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= tankTransform;

		//states.shader = shaderptr;
		target.draw(hull, states);
		target.draw(gun);
	}

	void	SetGunOnHull()
	{
		gun.setPosition(tankTransform.transformPoint(0, 5));
	}

public:
	Transform		tankTransform;
	Transform		lastTankTransform;
	TankState		tankState = TankAlive;
	RectangleShape	hull;
	Sprite			gun;

	const char*		name;
	TankTag			tankTag;

	int				currentShell = 0;
	int				maxShell = 9999;
	float			lastShootingTime = 0;

	Tank(const char* _name, TankTag _tankTag, Vector2f _pos, Shader* _shader, Texture* _hull, Texture* _gun, vector<Texture*> _explosionTexture, vector<SoundBuffer*> _soundBufferVec)
	{
		tankTransform = Transform::Identity;
		tankTransform.translate(_pos);
		name = _name;
		tankTag = _tankTag;

		hull.setSize(Vector2f(50, 70));
		hull.setOrigin(Vector2f(hull.getSize().x / 2, hull.getSize().y / 2));
		hull.setTexture(_hull);

		gun.setTexture(*_gun);
		gun.setOrigin(Vector2f(hull.getSize().x / 2 + 20, 170));
		gun.scale(0.3, 0.3);
		SetGunOnHull();
		shaderptr = _shader;
		explosionTexture = _explosionTexture;
		soundBufferVec = _soundBufferVec;

	};

	void		SetGunAngle(Vector2f mouseWorldPos)
	{
		float result = atan2f(mouseWorldPos.y - gun.getPosition().y, mouseWorldPos.x - gun.getPosition().x) * 180 / 3.14159265;
		gun.setRotation(result + 90);
	}

	void		MoveTank(MoveDirection _direction)
	{
		if (tankState == TankState::TankAlive)
		{
			switch (_direction)
			{
			case Up:
			{
				tankTransform.translate(0, -1 * tankSpeed);
				lastState = Up;
				break;
			}
			case RotateLeft:
				if (lastState == Up || lastState == Idle)
					tankTransform.rotate(-1);
				else if (lastState == Down)
					tankTransform.rotate(1);
				break;
			case RotateRight:
				if (lastState == Up || lastState == Idle)
					tankTransform.rotate(1);
				else if (lastState == Down)
					tankTransform.rotate(-1);
				break;
			case Down:
				tankTransform.translate(0, 1 * tankSpeed);
				lastState = Down;
				break;
			case Idle:
				lastState = Idle;
				break;
			default:
				break;
			}
			SetGunOnHull();
		}
	}

	FloatRect	GetTankGlobalBounds()
	{
		return tankTransform.transformRect(hull.getGlobalBounds());
	}

	Vector2f	GetTransformPosition()
	{
		return (tankTransform.transformPoint(0, 0));
	}

	bool		CheckIfCollideWithWall(Wall& wall)
	{
		if (GetTankGlobalBounds().intersects(wall.wall.getGlobalBounds()))
		{
			tankTransform = lastTankTransform;
			return true;
		}
		return false;
	}

	bool		CheckIfCollideWithOtherTank(Tank& otherTank)
	{
		if (this == &otherTank)
			return false;
		if (otherTank.tankState != TankState::TankDestroy)
			if (GetTankGlobalBounds().intersects(otherTank.GetTankGlobalBounds()))
			{
				tankTransform = lastTankTransform;
				return true;
			}
		return false;
	}

	void		DoExplosion(float time)
	{
		if ((time - lastExplosionTime) > 0.05f || lastExplosionTime == 0)
		{
			if (currentExplosion == 0)
			{
				tankSound.setBuffer(*soundBufferVec[0]);
				tankSound.play();
				hull.setScale(0.5f, 0.5f);
				gun.setScale(0.5f, 0.5f);
			}
			if (currentExplosion < 9)
			{
				hull.setTexture(explosionTexture[currentExplosion], true);
				hull.setOrigin(hull.getTexture()->getSize().x / 2, hull.getTexture()->getSize().y / 2);

				gun.setTexture(*explosionTexture[currentExplosion], true);
				gun.setOrigin(gun.getTexture()->getSize().x / 2, gun.getTexture()->getSize().y / 2);
				lastExplosionTime = time;
				currentExplosion++;
			}
			else
				tankState = TankState::TankDestroy;
		}
	}
};