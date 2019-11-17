#include <string>
#include <SFML/Graphics.hpp>
#include <iostream> 
#include <list>
#include <iterator> 
#include "Lib.h"
#include "Data.h"
#include "Tank.h"
#include "Wall.h"
#include "Shell.h"

using namespace sf;
using namespace std;

static Data _data;

static Vector2f screenSize(800, 600);

float offsetSpeed = 1.0f;

Vector2f mouseWorldPos;

void AddShell(Tank& tank, Vector2f& pos, float time)
{
	//if (tank.CurrentShell < tank.MaxShell && ((time - tank.lastShootingTime) > 1.0f || tank.lastShootingTime == 0))
	//{

		tank.lastShootingTime = time;
		tank.CurrentShell++;
		tank.CreateShell(pos);
		printf("Shoot\n");
	//}
}

void ExcecuteShell(RenderWindow& win)
{
	for (Tank& tank : _data.tankList)
	{
		for (Shell& elem : tank.shellList)
		{
			elem.shell.move(elem.xVelocity * 5, elem.yVelocity * 5);
		}
		tank.RemoveShell();
	}
	_data.RemoveTank();
}

void DrawCrosshair(RenderWindow& win)
{
	sf::VertexArray va(sf::LineStrip);
	int nb = 10;
	std::vector<Vector2f> points;

	points.push_back(Vector2f(_data.tankList[0].tank.getPosition().x, _data.tankList[0].tank.getPosition().y));
	points.push_back(Vector2f(mouseWorldPos.x, mouseWorldPos.y));


	for (int i = 0; i < nb + 1; ++i) {
		double ratio = 1.0 * i / nb;
		double x = 0.0;
		double y = 0.0;
		Vector2f pos = Lib::plot2(ratio, points);
		x = pos.x;
		y = pos.y;
		sf::Vertex vertex(Vector2f(x, y), i % 2 == 0 ? Color::Black : Color::Blue);
		va.append(vertex);
	}
	win.draw(va);

	CircleShape crosshair(3);
	crosshair.setOrigin(3 / 2, 3 / 2);
	crosshair.setPosition(Vector2f(mouseWorldPos.x, mouseWorldPos.y));
	_data.tankList[0].SetGunAngle(mouseWorldPos);
	win.draw(crosshair);
}

void DrawElement(RenderWindow& win)
{
	for (Tank& tank : _data.tankList)
	{
		win.draw(tank.tank);
		tank.SetGunToTankPosition();
		win.draw(tank.gun);
		for (Shell& shell : tank.shellList)
			win.draw(shell.shell);
	}
	for (Wall& elemen : _data.wallList)
		win.draw(elemen.wall);

}

void World(RenderWindow& win, float time)
{
	for (Tank& tank : _data.tankList)
	{
		if (tank.name != "Player")
		{
			if (_data.tankList[0].tank.getGlobalBounds().intersects(tank.tank.getGlobalBounds()))
			{
				_data.tankList[0].tank.setPosition(_data.tankList[0].lastposition);
				offsetSpeed = 0;
			}
		}
		for (Shell& shell : tank.shellList)
		{
			for (Tank& tankTarget : _data.tankList)
			{
				if (shell.shell.getGlobalBounds().intersects(tankTarget.tank.getGlobalBounds()))
				{
					if (shell.shooterName != tankTarget.name)
					{
						shell.Explode = true;
						tankTarget.IsAlive = false;
						printf("Shooter : %s Hit %s\n", tank.name, tankTarget.name);
					}
				}
			}
			//
			for (Wall& wall : _data.wallList)
			{
				if (shell.shell.getGlobalBounds().intersects(wall.wall.getGlobalBounds()))
				{
					if (wall.axe == Axe::Horizontale)
						shell.yVelocity = -shell.yVelocity;
					else if (wall.axe == Axe::Verticale)
						shell.xVelocity = -shell.xVelocity;
				}
			}
		}
	}
	for (Wall& wall : _data.wallList)
	{
		if (_data.tankList[0].tank.getGlobalBounds().intersects(wall.wall.getGlobalBounds()))
		{
			_data.tankList[0].tank.setPosition(_data.tankList[0].lastposition);
			offsetSpeed = 0;
		}
	}

	_data.tankList[0].lastposition = _data.tankList[0].tank.getPosition();
	offsetSpeed = 5;
	ExcecuteShell(win);
	DrawElement(win);
	DrawCrosshair(win);

}

int main()
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

	_data.AddWall("WallUP", Axe::Horizontale, Vector2f(0, 0), Vector2f(800, 20));
	_data.AddWall("WallLeft", Axe::Verticale, Vector2f(0, 0), Vector2f(20, 600));
	_data.AddWall("WallRight", Axe::Verticale, Vector2f(screenSize.x - 20, 0), Vector2f(20, screenSize.y));
	_data.AddWall("WallDown", Axe::Horizontale, Vector2f(0, screenSize.y - 20), Vector2f(screenSize.x, 20));

	_data.AddTank("Player", Vector2f(50, 50), Vector2f(30, 30), Color::Blue);
	_data.AddTank("Bot 1", Vector2f(400, 80), Vector2f(30, 30), Color::Red);
	_data.AddTank("Bot 2", Vector2f(400, 160), Vector2f(30, 30), Color::Red);

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

		mousePos = Mouse::getPosition(window);
		mouseWorldPos = window.mapPixelToCoords(mousePos);
		if (Keyboard::isKeyPressed(Keyboard::Right))_data.tankList[0].Move(Direction::Right, offsetSpeed);
		if (Keyboard::isKeyPressed(Keyboard::Left))_data.tankList[0].Move(Direction::Left, offsetSpeed);
		if (Keyboard::isKeyPressed(Keyboard::Up))_data.tankList[0].Move(Direction::Up, offsetSpeed);
		if (Keyboard::isKeyPressed(Keyboard::Down))_data.tankList[0].Move(Direction::Down, offsetSpeed);
		if (Keyboard::isKeyPressed(Keyboard::Space))AddShell(_data.tankList[0], mouseWorldPos, frameStart.asSeconds());

		window.clear();

		World(window, frameStart.asSeconds());

		fpsText.setString(to_string(fps));
		mousePosText.setString(to_string(mouseWorldPos.x) + " " + to_string(mouseWorldPos.y));
		playerPosText.setString(to_string(_data.tankList[0].tank.getPosition().x) + " " + to_string(_data.tankList[0].tank.getPosition().y));

		window.draw(playerPosText);
		window.draw(mousePosText);
		window.draw(fpsText);

		window.display();

		frameEnd = clock.getElapsedTime();
		lastFrame = frameStart;
	}
	return 0;
}