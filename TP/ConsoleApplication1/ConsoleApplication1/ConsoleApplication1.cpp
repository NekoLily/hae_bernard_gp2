#include <string>
#include <SFML/Graphics.hpp>
#include <iostream> 
#include <list>
#include <iterator> 
#include "Data.h"
#include "Tank.h"
#include "Wall.h"
#include "Shell.h"
#include "UI.h"

using namespace sf;
using namespace std;

int			fps;
Data		_data;
Vector2f	screenSize(800, 600);
Font* font = new Font();

float		WallSize = 50;
float		offSetSpeed = 0.5f;
float		offSetCrosshairRange = 70;

Vector2f	mouseWorldPos;

GameState gameState;
MenuState menuState;



void		ShowDebug(RenderWindow& win, int fps)
{
	Text	fpsText;
	Text	mousePosText;
	Text	playerPosText;

	fpsText.setFont(*font);
	fpsText.setFillColor(Color::Red);

	playerPosText.setPosition(screenSize.x / 2, 0);
	playerPosText.setFont(*font);
	playerPosText.setFillColor(Color::Red);

	mousePosText.setPosition(screenSize.x - screenSize.x / 3, 0);
	mousePosText.setFont(*font);
	mousePosText.setFillColor(Color::Red);

	fpsText.setString(to_string(fps));
	mousePosText.setString(to_string((int)mouseWorldPos.x) + " " + to_string((int)mouseWorldPos.y));
	if (_data.tankList.empty() == false)
		playerPosText.setString(to_string((int)_data.tankList[0].tank.getPosition().x) + " " + to_string((int)_data.tankList[0].tank.getPosition().y));

	win.draw(playerPosText);
	win.draw(mousePosText);
	win.draw(fpsText);
}

void		InitMenu()
{
	switch (menuState)
	{
	case MainMenu:
	{
		Button singlePlayerButton = Button(470, 70, String("Single Player"), Vector2f(screenSize.x - screenSize.x / 1.3, screenSize.y / 3), Color::Black, font, Color::Green, MenuState::SinglePlayerMenu);
		_data.ButtonList.push_back(singlePlayerButton);
		Button MultiPlayerButton = Button(470, 70, String("Versus"), Vector2f(screenSize.x - screenSize.x / 1.3, screenSize.y / 2), Color::Black, font, Color::Green, MenuState::MultiPlayerMenu);
		_data.ButtonList.push_back(MultiPlayerButton);
	}
	break;
	case SinglePlayerMenu:
		break;
	case MultiPlayerMenu:
		break;
	case InGameMenu:
		break;
	case PauseMenu:
	{
		Button quitButton(70, 30, String("Quit"), Vector2f(screenSize.x / 2 - screenSize.x / 5, screenSize.y / 1.5), Color::Black, font, Color::Green, MenuState::MainMenu);
		_data.ButtonList.push_back(quitButton);
		Button retryButton(70, 30, String("Retry"), Vector2f(screenSize.x / 2 + screenSize.x / 5, screenSize.y / 1.5), Color::Black, font, Color::Green, MenuState::Reload);
		_data.ButtonList.push_back(retryButton);
	}
		break;
	case WinMenu:
		break;
	case LooseMenu:
	{
		Button quitButton(70, 30, String("Quit"), Vector2f(screenSize.x / 2 - screenSize.x / 5, screenSize.y / 1.5), Color::Black, font, Color::Green, MenuState::MainMenu);
		_data.ButtonList.push_back(quitButton);
		Button retryButton(70, 30, String("Retry"), Vector2f(screenSize.x / 2 + screenSize.x / 5, screenSize.y / 1.5), Color::Black, font, Color::Green, MenuState::Reload);
		_data.ButtonList.push_back(retryButton);
	}
		break;
	default:
		break;
	}
}

void		DrawMenu(RenderWindow& win)
{
	switch (menuState)
	{
	case MainMenu:
	{
		CustomText		titleText = CustomText(100, font, Vector2f(screenSize.x /2, screenSize.y / 10), Color::Black, String("MainMenu"));
		win.draw(titleText);
		for (Button& button : _data.ButtonList)
			button.DrawButton(win);
	}
	break;
	case SinglePlayerMenu:
		break;
	case MultiPlayerMenu:
		break;
	case InGameMenu:
		break;
	case PauseMenu:
	{
		CustomText	pauseText = CustomText(100, font, Vector2f(screenSize.x / 2, screenSize.y / 2), Color::Black, String("Pause"));
		win.draw(pauseText);
		for (Button& button : _data.ButtonList)
			button.DrawButton(win);
	}
	break;
	case WinMenu:
	{
		CustomText winText = CustomText(100, font, Vector2f(screenSize.x / 2, screenSize.y / 2), Color::Black, String("Win"));
		win.draw(winText);
		for (Button& button : _data.ButtonList)
			button.DrawButton(win);
		break;
	}
	break;
	case LooseMenu:
	{
		CustomText looseText = CustomText(100, font, Vector2f(screenSize.x / 2, screenSize.y / 2), Color::Black, String("Loose"));
		win.draw(looseText);
		for (Button& button : _data.ButtonList)
			button.DrawButton(win);
		break;
	}
	break;
	default:
		break;
	}
}

void		AddShell(Tank& tank, Vector2f pos, float time)
{
	if (tank.currentShell < tank.maxShell && ((time - tank.lastShootingTime) > 0.5f || tank.lastShootingTime == 0))
	{
		tank.lastShootingTime = time;
		tank.currentShell++;
		_data.AddShell(tank, pos, 4);
	}
}

void		ExcecuteShell(RenderWindow& win)
{
	for (Shell& elem : _data.shellList)
		elem.shell.move(elem.xDirection * elem.offSetSpeed, elem.yDirection * elem.offSetSpeed);
	_data.RemoveShell();
	_data.RemoveTank();
}

void		DrawCrosshair(RenderWindow& win)
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

void		DrawElement(RenderWindow& win)
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

void		World(RenderWindow& win, float time)
{
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
					shell.CurrentHit++;
					if (shell.CurrentHit == shell.maxHit)
					{
						shell.Explode = true;
						_data.DecreaseCurrentShell(shell.shooterName);
					}
					else
						shell.CheckCollisionSide(wall);
				}
				if (shell.Explode)
					break;
			}
		}
		offSetSpeed = 5;
		ExcecuteShell(win);
		if (_data.tankList.empty() == false)
		{
			if (_data.tankList[0].name == "Player")
			{
				DrawCrosshair(win);
				if (_data.tankList.size() == 1)
				{
					gameState = GameState::End;
					menuState = MenuState::WinMenu;
				}

			}
			else
			{
				gameState = GameState::End;
				menuState = MenuState::LooseMenu;
			}
		}
		else
		{
			gameState = GameState::End;
			menuState = MenuState::LooseMenu;
		}
	}
}

int	main()
{
	ContextSettings		settings;
	RenderWindow		window(VideoMode(screenSize.x, screenSize.y), "SFML works!");
	Clock				clock;
	Time				time = clock.getElapsedTime();
	Time				frameStart;
	Time				frameEnd;
	Time				lastFrame;
	Vector2i			mousePos;

	bool				isPause = false;
	bool				isMouseCursorIsVisible = true;

	settings.antialiasingLevel = 10;
	window.setMouseCursorVisible(true);
	window.setVerticalSyncEnabled(true);

	font->loadFromFile("C:\\Windows\\Fonts\\arial.ttf");

	gameState = GameState::Start;
	menuState = MenuState::MainMenu;

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
				switch (gameState)
				{
				case Menu:
					break;
				case Playing:
					if (event.key.code == Keyboard::Space)
					{
						isPause = true;
						gameState = GameState::Pause;
						menuState = MenuState::PauseMenu;
						InitMenu();
					}
					else if (event.type == Event::LostFocus)
					{
						isPause = true;
						gameState = GameState::Pause;
						menuState = MenuState::PauseMenu;
						InitMenu();
					}
					break;
				case Pause:
					if (event.key.code == Keyboard::Space)
					{
						isPause = true;
						gameState = GameState::Playing;
						_data.ButtonList.clear();
					}
					break;
				default:
					break;
				}

			}
			if (event.type == Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == Mouse::Left)
				{
					//CircleShape mouseClick(5);
					//mouseClick.setOrigin(Vector2f(mouseClick.getRadius(), mouseClick.getRadius()));
					//mouseClick.setPosition(mouseWorldPos);
					for (Button& button : _data.ButtonList)
					{
						if (button.CheckButton(mouseWorldPos))
						{
							menuState = button.state;
							_data.ButtonList.clear();
							switch (menuState)
							{
							case MainMenu:
								gameState = GameState::Start;
								_data.ClearData();
								break;
							case SinglePlayerMenu:
								gameState = GameState::Loading;
								break;
							case MultiPlayerMenu:
								break;
							case InGameMenu:
								break;
							case PauseMenu:
								break;
							case WinMenu:
								break;
							case LooseMenu:
								break;
							case Reload:
								_data.ClearData();
								gameState = GameState::Loading;
								break;
							case Null:
								break;
							default:
								break;
							}
						}

					}
				}

			}
			if (event.type == Event::Closed)
				window.close();
		}

		mousePos = Mouse::getPosition(window);
		mouseWorldPos = window.mapPixelToCoords(mousePos);

		switch (gameState)
		{
		case Start:
			InitMenu();
			gameState = GameState::Menu;
			break;
		case Menu:
			DrawMenu(window);
			break;
		case Loading:
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
			menuState = MenuState::PauseMenu;
			break;
		case Playing:
			if (isMouseCursorIsVisible == true) { isMouseCursorIsVisible = false; window.setMouseCursorVisible(false); }
			if (Keyboard::isKeyPressed(Keyboard::Right))_data.tankList[0].Move(MoveDirection::Right, offSetSpeed);
			if (Keyboard::isKeyPressed(Keyboard::Left))_data.tankList[0].Move(MoveDirection::Left, offSetSpeed);
			if (Keyboard::isKeyPressed(Keyboard::Up))_data.tankList[0].Move(MoveDirection::Up, offSetSpeed);
			if (Keyboard::isKeyPressed(Keyboard::Down))_data.tankList[0].Move(MoveDirection::Down, offSetSpeed);
			if (Mouse::isButtonPressed(Mouse::Left))AddShell(_data.tankList[0], mouseWorldPos, frameStart.asSeconds());
			DrawElement(window);
			World(window, frameStart.asSeconds());
			break;
		case Pause:
			DrawElement(window);
			DrawMenu(window);
			if (isMouseCursorIsVisible == false) { isMouseCursorIsVisible = true; window.setMouseCursorVisible(true); }
			break;
		case End:
			InitMenu();
			DrawElement(window);
			DrawMenu(window);
			if (isMouseCursorIsVisible == false) { isMouseCursorIsVisible = true; window.setMouseCursorVisible(true); }
			break;
		default:
			break;
		}
		//ShowDebug(window, fps);
		window.display();

		frameEnd = clock.getElapsedTime();
		lastFrame = frameStart;
	}
	return 0;
}