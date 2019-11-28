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

int			fps;
Data		_data;
Vector2f	screenSize(800, 600);
Font		font;

float		WallSize = 50;
float		offSetSpeed = 0.5f;
float		offSetCrosshairRange = 70;

Vector2f	mouseWorldPos;

GameState gameState;

Direction	CheckCollisionSide(Shell shell, Wall wall)
{
	Rect<float> shellRect = shell.shell.getGlobalBounds();
	Rect<float> wallRect = wall.wall.getGlobalBounds();

	float shellRectRight = shellRect.left + shellRect.width;
	float shellRectBot = shellRect.top + shellRect.height;
	float wallRectRight = wallRect.left + wallRect.width;
	float walllRectBot = wallRect.top + wallRect.height;

	Direction	Side = Direction::Null;

	//printf("shell L:%f R:%f T:%f B:%f\n", shellRect.left, shellRectRight, shellRect.top, shellRectBot);
	//printf("wall : %s L:%f R:%f  T:%f B:%f W:%f H:%f\n",wall.name,  wallRect.left, wallRectRight, wallRect.top, walllRectBot, wallRect.width, wallRect.height);

	if (shellRectRight > wallRect.left&& shellRect.left < wallRect.left && shell.xDirection > 0)
	{
		//printf("Left\n");
		Side = Direction::Left;
	}
	else if (shellRect.left <= wallRectRight && shellRectRight - 1 >= wallRectRight - 1 && shell.xDirection < 0)
	{
		//printf("Right\n");
		Side = Direction::Right;
	}
	else if (shellRectBot >= wallRect.top && shellRect.top <= wallRect.top)
	{
		//printf("UpSide\n");
		Side = Direction::Up;
	}
	else if (shellRect.top <= walllRectBot && shellRectBot >= walllRectBot)
	{
		//printf("BotSide\n");
		Side = Direction::Down;
	}
	//printf("\n");
	return Side;
}

void	ShowDebug(RenderWindow& win, int fps)
{
	Text	fpsText;
	Text	mousePosText;
	Text	playerPosText;

	fpsText.setFont(font);
	fpsText.setFillColor(Color::Red);

	playerPosText.setPosition(screenSize.x / 2, 0);
	playerPosText.setFont(font);
	playerPosText.setFillColor(Color::Red);

	mousePosText.setPosition(screenSize.x - screenSize.x / 3, 0);
	mousePosText.setFont(font);
	mousePosText.setFillColor(Color::Red);

	fpsText.setString(to_string(fps));
	mousePosText.setString(to_string((int)mouseWorldPos.x) + " " + to_string((int)mouseWorldPos.y));
	if (_data.tankList.empty() == false)
		playerPosText.setString(to_string((int)_data.tankList[0].tank.getPosition().x) + " " + to_string((int)_data.tankList[0].tank.getPosition().y));

	win.draw(playerPosText);
	win.draw(mousePosText);
	win.draw(fpsText);
}

void	ShowMessage(RenderWindow& win)
{
	Text	text;

	switch (gameState)
	{
	case Menu:
		break;
	case Playing:
		break;
	case Pause:
		text.setCharacterSize(100);
		text.setFont(font);
		text.setPosition(screenSize.x - screenSize.x / 1.3, screenSize.y / 2);
		text.setFillColor(Color::Red);
		text.setString("Pause");
		break;
	case Loose:
		text.setCharacterSize(100);
		text.setFont(font);
		text.setPosition(screenSize.x - screenSize.x / 1.3, screenSize.y / 2);
		text.setFillColor(Color::Red);
		text.setString("You loose !");
		break;
	case Win:
		text.setCharacterSize(100);
		text.setFont(font);
		text.setPosition(screenSize.x - screenSize.x / 1.3, screenSize.y / 2);
		text.setFillColor(Color::Green);
		text.setString("You win !");
		break;
	default:
		break;
	}
	win.draw(text);
}

void	AddShell(Tank& tank, Vector2f pos, float time)
{
	if (tank.currentShell < tank.maxShell && ((time - tank.lastShootingTime) > 0.5f || tank.lastShootingTime == 0))
	{
		tank.lastShootingTime = time;
		tank.currentShell++;
		_data.AddShell(tank, pos, 4);
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
	float		xDistance = tankPosition.x - mouseWorldPos.x;
	float		yDistance = tankPosition.y - mouseWorldPos.y;
	float		Distance = sqrt(xDistance * xDistance + yDistance * yDistance);

	float		xDirection = -xDistance / Distance;
	float		yDirection = -yDistance / Distance;

	CircleShape		dot(5);
	RectangleShape	HorCross(Vector2f(10, 2));
	RectangleShape	VerCross(Vector2f(2, 10));

	HorCross.setOrigin(10 / 2, 2 / 2);
	HorCross.setFillColor(Color::Red);
	//HorCross.setPosition(tankPosition.x + xDirection * offSetCrosshairRange, tankPosition.y + yDirection * offSetCrosshairRange);
	HorCross.setPosition(mouseWorldPos);

	VerCross.setOrigin(2 / 2, 10 / 2);
	VerCross.setFillColor(Color::Red);
	//VerCross.setPosition(tankPosition.x + xDirection * offSetCrosshairRange, tankPosition.y + yDirection * offSetCrosshairRange);
	VerCross.setPosition(mouseWorldPos);

	dot.setOrigin(5, 5);
	dot.setFillColor(Color::Transparent);
	dot.setOutlineThickness(1);
	dot.setOutlineColor(Color::Red);
	//dot.setPosition(tankPosition.x + xDirection * offSetCrosshairRange, tankPosition.y + yDirection * offSetCrosshairRange);
	dot.setPosition(mouseWorldPos);

	_data.tankList[0].SetGunAngle(mouseWorldPos);
	win.draw(dot);
	win.draw(HorCross);
	win.draw(VerCross);
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
		win.draw(tank.circleGun);
	}
	for (Wall& elemen : _data.wallList)
		win.draw(elemen.wall);
}

void	World(RenderWindow& win, float time)
{
	DrawElement(win);
	if (gameState == GameState::Playing)
	{
		
		for (Tank& tank : _data.tankList)
		{
			if (tank.name != "Player")
			{
				tank.SetGunAngle(_data.tankList[0].tank.getPosition());
				AddShell(tank, _data.tankList[0].tank.getPosition(), time);
			}
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
					_data.DecreaseCurrentShell(shell.shooterName);
					printf("Shooter : %s Hit %s\n", shell.shooterName, tankTarget.name);
				}
			}
			for (Wall& wall : _data.wallList)
			{
				if (shell.shell.getGlobalBounds().intersects(wall.wall.getGlobalBounds()))
				{
					Direction side = CheckCollisionSide(shell, wall);
					shell.CurrentHit++;
					if (shell.CurrentHit == shell.maxHit)
					{
						shell.Explode = true;
						_data.DecreaseCurrentShell(shell.shooterName);
					}
					else
						switch (side)
						{
						case Up:
							if (shell.yDirection > 0)
								shell.yDirection = -shell.yDirection;
							break;
						case Left:
							if (shell.xDirection > 0)
								shell.xDirection = -shell.xDirection;
							break;
						case Right:
							if (shell.xDirection < 0)
								shell.xDirection = -shell.xDirection;
							break;
						case Down:
							if (shell.yDirection < 0)
								shell.yDirection = -shell.yDirection;
							break;
						}
					
				}
			}
			offSetSpeed = 5;

		}
		ExcecuteShell(win);
		if (_data.tankList.empty() == false)
		{
			if (_data.tankList[0].name == "Player")
			{
				DrawCrosshair(win);
				if (_data.tankList.size() == 1)
					gameState = GameState::Win;
			}
			else
				gameState = GameState::Loose;
		}
		else
			gameState = GameState::Loose;
	}
}

int	main()
{
	ContextSettings		settings;
	settings.antialiasingLevel = 10;
	RenderWindow		window(VideoMode(screenSize.x, screenSize.y), "SFML works!");
	window.setMouseCursorVisible(true);
	window.setVerticalSyncEnabled(true);

	Clock	clock;
	Time	time = clock.getElapsedTime();
	Time	frameStart;
	Time	frameEnd;
	Time	lastFrame;

	font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");


	Vector2i	mousePos;
	bool		isPause = false;
	bool		isMouseCursorIsVisible = true;

	_data.AddWall("WallMap", Vector2f(0, 0), Vector2f(screenSize.x, WallSize));
	_data.AddWall("WallMap", Vector2f(0, 0), Vector2f(WallSize, screenSize.y));
	_data.AddWall("WallMap", Vector2f(screenSize.x - WallSize, 0), Vector2f(WallSize, screenSize.y));
	_data.AddWall("WallMap", Vector2f(0, screenSize.y - WallSize), Vector2f(screenSize.x, WallSize));
	_data.AddWall("Obstacle verticale", Vector2f(screenSize.x / 2, screenSize.y / 2), Vector2f(50, 100));
	_data.AddWall("Obstacle horizontale", Vector2f(screenSize.x / 2 - 100, screenSize.y / 2 + 100), Vector2f(150, 50));
	_data.AddWall("Obstacle carre", Vector2f(screenSize.x / 2 + 200, screenSize.y / 2), Vector2f(50, 50));
	_data.AddTank("Player", Vector2f(screenSize.x / 2, 500), Vector2f(30, 30), Color::Blue);
	_data.AddTank("Bot 1", Vector2f(300, 80), Vector2f(30, 30), Color::Red);
	_data.AddTank("Bot 2", Vector2f(500, 80), Vector2f(30, 30), Color::Red);

	gameState = GameState::Pause;

	while (window.isOpen())
	{
		window.clear(Color(212, 192, 171, 255));
		frameStart = clock.getElapsedTime();
		fps = 1.0f / (frameStart - lastFrame).asSeconds();

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyPressed)
			{
				if (event.key.code == Keyboard::Space)
				{
					isPause = !isPause;
					if (isPause)
						gameState = GameState::Pause;
					else
						gameState = GameState::Playing;
				}
				else if (event.type == Event::LostFocus)
				{
					isPause = true;
					gameState = GameState::Pause;
				}
			}
			if (event.type == Event::Closed)
				window.close();
		}

		mousePos = Mouse::getPosition(window);
		mouseWorldPos = window.mapPixelToCoords(mousePos);

		if (gameState == GameState::Playing)
		{
			if (isMouseCursorIsVisible == true) { isMouseCursorIsVisible = false; window.setMouseCursorVisible(false); }
			if (Keyboard::isKeyPressed(Keyboard::Right))_data.tankList[0].Move(Direction::Right, offSetSpeed);
			if (Keyboard::isKeyPressed(Keyboard::Left))_data.tankList[0].Move(Direction::Left, offSetSpeed);
			if (Keyboard::isKeyPressed(Keyboard::Up))_data.tankList[0].Move(Direction::Up, offSetSpeed);
			if (Keyboard::isKeyPressed(Keyboard::Down))_data.tankList[0].Move(Direction::Down, offSetSpeed);
			if (Mouse::isButtonPressed(Mouse::Left))AddShell(_data.tankList[0], mouseWorldPos, frameStart.asSeconds());
		}
		else
			if (isMouseCursorIsVisible == false) { isMouseCursorIsVisible = true; window.setMouseCursorVisible(true); }

		World(window, frameStart.asSeconds());
		//ShowDebug(window, fps);
		ShowMessage(window);
		window.display();

		frameEnd = clock.getElapsedTime();
		lastFrame = frameStart;
	}
	return 0;
}