#include <string>
#include <SFML/Graphics.hpp>
#include <iostream> 
#include <list>
#include <iterator> 
#include "Data.h"
#include "Tank.h"
#include "Wall.h"
#include "Shell.h"

using namespace sf;
using namespace std;

Data _data;

Vector2f screenSize(800, 600);

float WallSize = 50;
float offSetSpeed = 1.0f;
float offSetCrosshairRange = 50;

Vector2f _pos;

void	AddShell(Tank& tank, Vector2f& pos, float time)
{
	if (tank.currentShell < tank.maxShell && ((time - tank.lastShootingTime) > 0.5f || tank.lastShootingTime == 0))
	{
		tank.lastShootingTime = time;
		tank.currentShell++;
		_data.AddShell(tank, pos, 2);
		printf("Shoot\n");
	}
}

void	ExcecuteShell(RenderWindow& win)
{
	for (Shell& elem : _data.shellList)
		elem.shell.move(elem.xDirection * elem.offSetSpeed, elem.yDirection * elem.offSetSpeed);
	_data.RemoveShell();
	_data.RemoveTank();
}

void	DrawCrosshair(RenderWindow& win)
{
	Vector2f	tankPosition = _data.tankList[0].tank.getPosition();
	float		xDistance = tankPosition.x - _pos.x;
	float		yDistance = tankPosition.y - _pos.y;
	float		Distance = sqrt(xDistance * xDistance + yDistance * yDistance);

	float		xDirection = -xDistance / Distance;
	float		yDirection = -yDistance / Distance;

	CircleShape crosshair(3);
	crosshair.setOrigin(3 / 2, 3 / 2);
	crosshair.setPosition(tankPosition.x + xDirection * offSetCrosshairRange, tankPosition.y + yDirection * offSetCrosshairRange);
	_data.tankList[0].SetGunAngle(_pos);
	win.draw(crosshair);
}

void	DrawElement(RenderWindow& win)
{
	for (Shell& shell : _data.shellList)
		win.draw(shell.shell);
	for (Tank& tank : _data.tankList)
	{
		win.draw(tank.tank);
		tank.SetGunToTankPosition();
		win.draw(tank.gun);
	}
	for (Wall& elemen : _data.wallList)
		win.draw(elemen.wall);

}

void	World(RenderWindow& win, float time)
{
	for (Tank& tank : _data.tankList)
	{
		for (Tank& otherTank : _data.tankList)
			if (tank.name != otherTank.name && tank.tank.getGlobalBounds().intersects(otherTank.tank.getGlobalBounds()))
				tank.tank.setPosition(tank.lastposition);
		for (Wall& wall : _data.wallList)
		{
			if (tank.tank.getGlobalBounds().intersects(wall.wall.getGlobalBounds()))
			{
				_data.tankList[0].tank.setPosition(_data.tankList[0].lastposition);
				offSetSpeed = 0;
			}
		}
		tank.lastposition = tank.tank.getPosition();
	}

	for (Shell& shell : _data.shellList)
	{
		for (Tank& tankTarget : _data.tankList)
		{
			if (shell.shell.getGlobalBounds().intersects(tankTarget.tank.getGlobalBounds()))
			{
				shell.Explode = true;
				tankTarget.IsAlive = false;
				printf("Shooter : %s Hit %s\n", shell.shooterName, tankTarget.name);
			}
		}
		for (Wall& wall : _data.wallList)
		{
			if (shell.shell.getGlobalBounds().intersects(wall.wall.getGlobalBounds()))
			{
				if (wall.axe == Axe::Horizontale)
					shell.yDirection = -shell.yDirection;
				else if (wall.axe == Axe::Verticale)
					shell.xDirection = -shell.xDirection;
			}
		}
	}
	offSetSpeed = 5;
	ExcecuteShell(win);
	if (_data.tankList.empty() == false)
		DrawCrosshair(win);
	DrawElement(win);
}

int	main()
{
	ContextSettings settings;
	settings.antialiasingLevel = 2;
	RenderWindow window(VideoMode(screenSize.x, screenSize.y), "SFML works!");
	window.setVerticalSyncEnabled(true);


	Clock clock;
	Time time = clock.getElapsedTime();
	Time frameStart;
	Time frameEnd;
	Time lastFrame;

	Text fpsText;
	Text mousePosText;
	Text playerPosText;
	Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");

	fpsText.setFont(font);
	fpsText.setFillColor(Color::Red);

	playerPosText.setPosition(500, 0);
	playerPosText.setFont(font);
	playerPosText.setFillColor(Color::Red);

	mousePosText.setPosition(1000, 0);
	mousePosText.setFont(font);
	mousePosText.setFillColor(Color::Red);

	float fps;
	Vector2i mousePos;

	_data.AddWall("WallUP", Axe::Horizontale, Vector2f(0, 0), Vector2f(screenSize.x, WallSize));
	_data.AddWall("WallLeft", Axe::Verticale, Vector2f(0, 0), Vector2f(WallSize, screenSize.y));
	_data.AddWall("WallRight", Axe::Verticale, Vector2f(screenSize.x - WallSize, 0), Vector2f(WallSize, screenSize.y));
	_data.AddWall("WallDown", Axe::Horizontale, Vector2f(0, screenSize.y - WallSize), Vector2f(screenSize.x, WallSize));

	_data.AddTank("Player", Vector2f(100, 500), Vector2f(30, 30), Color::Blue);
	//_data.AddTank("Bot 1", Vector2f(400, 80), Vector2f(30, 30), Color::Red);
	//_data.AddTank("Bot 2", Vector2f(400, 160), Vector2f(30, 30), Color::Red);

	Direction direction;

	while (window.isOpen())
	{
		frameStart = clock.getElapsedTime();
		fps = 1.0f / (frameStart - lastFrame).asSeconds();

		Event event;
		while (window.pollEvent(event))
		{
			switch (event.key.code)
			{
			case Keyboard::E:
				printf("FPS : %f\n", fps);
				break;
			default:
				break;
			}
			if (event.type == Event::Closed)
				window.close();
		}
		window.clear();
		mousePos = Mouse::getPosition(window);
		_pos = window.mapPixelToCoords(mousePos);
		if (_data.tankList.empty() == false)
		{
			if (_data.tankList.empty() == false && _data.tankList[0].name == "Player")
			{
				if (Keyboard::isKeyPressed(Keyboard::Right))_data.tankList[0].Move(Direction::Right, offSetSpeed);
				if (Keyboard::isKeyPressed(Keyboard::Left))_data.tankList[0].Move(Direction::Left, offSetSpeed);
				if (Keyboard::isKeyPressed(Keyboard::Up))_data.tankList[0].Move(Direction::Up, offSetSpeed);
				if (Keyboard::isKeyPressed(Keyboard::Down))_data.tankList[0].Move(Direction::Down, offSetSpeed);
				if (Mouse::isButtonPressed(Mouse::Left))AddShell(_data.tankList[0], _pos, frameStart.asSeconds());
			}

		}
		World(window, frameStart.asSeconds());

		fpsText.setString(to_string(fps));
		//mousePosText.setString(to_string(mouseWorldPos.x) + " " + to_string(mouseWorldPos.y));
		//playerPosText.setString(to_string(_data.tankList[0].tank.getPosition().x) + " " + to_string(_data.tankList[0].tank.getPosition().y));

		window.draw(playerPosText);
		window.draw(mousePosText);
		window.draw(fpsText);

		window.display();

		frameEnd = clock.getElapsedTime();
		lastFrame = frameStart;
	}
	return 0;
}