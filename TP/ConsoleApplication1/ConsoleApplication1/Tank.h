#pragma once
#include <string>
#include <math.h>
#include <SFML/Graphics.hpp>
#include "Enum.h"
#include "Wall.h"

using namespace std;
using namespace sf;

class Tank : public Drawable, public Transformable
{
private:
	Shader* shaderptr;
	
	vector<Texture*> explosionTexture;

	int			currentExplosion = 0;
	float		lastExplosionTime = 0;
	
	float			tankSpeed = 2.f;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= tankTransform;

		//states.shader = shaderptr;
		target.draw(hull, states);
		target.draw(gun);
	}


public:
	Transform		tankTransform;
	Transform		lastTankTransform;
	TankState		tankState = Alive;

	RectangleShape	hull;
	Sprite			gun;

	const char*		name;
	int				currentShell = 0;
	int				maxShell = 3;
	float			lastShootingTime = 0;

	Tank(const char* _name, Vector2f _pos, Color _color, Shader *_shader, Texture *_hull, Texture * _gun, vector<Texture*> _explosionTexture)
	{
		tankTransform = Transform::Identity;
		tankTransform.translate(_pos);
		name = _name;

		hull.setSize(Vector2f(50, 70));
		hull.setOrigin(Vector2f(hull.getSize().x / 2, hull.getSize().y / 2));
		hull.setTexture(_hull);

		gun.setTexture(*_gun);
		gun.setOrigin(Vector2f(hull.getSize().x / 2 + 20,170));
		gun.scale(0.3, 0.3);
		shaderptr =_shader;
		explosionTexture = _explosionTexture;
	};

	void	SetGunAngle(Vector2f mouseWorldPos)
	{
		float result = atan2f(mouseWorldPos.y - gun.getPosition().y, mouseWorldPos.x - gun.getPosition().x) * 180 / 3.14159265;
		gun.setRotation(result + 90);
	}

	void	SetGunOnTank()
	{
		gun.setPosition(tankTransform.transformPoint(0, 5));
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
		return tankTransform.transformRect(hull.getGlobalBounds());
	}

	bool	CheckIfCollideWithWall(Wall wall)
	{
		if (GetTankGlobalBounds().intersects(wall.wall.getGlobalBounds()))
		{
			tankTransform = lastTankTransform;
			return true;
		}
		return false;
	}

	bool	CheckIfCollideWithOtherTank(Tank otherTank)
	{
		if (GetTankGlobalBounds().intersects(otherTank.GetTankGlobalBounds()))
		{
			tankTransform = lastTankTransform;
			return true;
		}
		return false;
	}

	void	DoExplosion(float time)
	{
		if ((time - lastExplosionTime) > 0.05f || lastExplosionTime == 0)
		{
			if (currentExplosion == 0)
			{
				hull.setScale(0.5f, 0.5f);
				gun.setScale(0.5f, 0.5f);
			}	
			if (currentExplosion < 8)
			{
				hull.setTexture(explosionTexture[currentExplosion], true);
				hull.setOrigin(hull.getTexture()->getSize().x / 2, hull.getTexture()->getSize().y / 2);

				gun.setTexture(*explosionTexture[currentExplosion], true);
				gun.setOrigin(gun.getTexture()->getSize().x / 2, gun.getTexture()->getSize().y / 2);
				lastExplosionTime = time;
				currentExplosion++;
			}
			else
				tankState = TankState::Die;
		}
	}
};