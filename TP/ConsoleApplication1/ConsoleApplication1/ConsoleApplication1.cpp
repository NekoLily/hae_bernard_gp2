#include <string>
#include <SFML/Graphics.hpp>
#include <iostream> 
#include <list>
#include <iterator> 
#include <string>
#include "Game.h"
#include "Tank.h"
#include "Wall.h"
#include "Shell.h"
#include "UI.h"

using namespace sf;
using namespace std;

int			fps;
Game		game;
Vector2f	screenSize(800, 600);

static Font* font = new Font();
static Shader* simpleShader = new Shader();

static Texture* blueHull = new Texture();
static Texture* blueGun = new Texture();
static Texture* lightShell = new Texture();

static vector<Texture*> explosionTexture;
static vector<Texture*> blueTankTexture;
static vector<Texture*> brownTankTexture;

float		WallSize = 50;
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
	if (game.tankList.empty() == false)
		playerPosText.setString(to_string((int)game.tankList[0].hull.getPosition().x) + " " + to_string((int)game.tankList[0].hull.getPosition().y));

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
		game.ButtonList.push_back(singlePlayerButton);
		Button MultiPlayerButton = Button(470, 70, String("Versus"), Vector2f(screenSize.x - screenSize.x / 1.3, screenSize.y / 2), Color::Black, font, Color::Green, MenuState::MultiPlayerMenu);
		game.ButtonList.push_back(MultiPlayerButton);
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
		game.ButtonList.push_back(quitButton);
		Button retryButton(70, 30, String("Retry"), Vector2f(screenSize.x / 2 + screenSize.x / 5, screenSize.y / 1.5), Color::Black, font, Color::Green, MenuState::Reload);
		game.ButtonList.push_back(retryButton);
	}
	break;
	case WinMenu:
		break;
	case LooseMenu:
	{
		Button quitButton(70, 30, String("Quit"), Vector2f(screenSize.x / 2 - screenSize.x / 5, screenSize.y / 1.5), Color::Black, font, Color::Green, MenuState::MainMenu);
		game.ButtonList.push_back(quitButton);
		Button retryButton(70, 30, String("Retry"), Vector2f(screenSize.x / 2 + screenSize.x / 5, screenSize.y / 1.5), Color::Black, font, Color::Green, MenuState::Reload);
		game.ButtonList.push_back(retryButton);
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
		CustomText		titleText = CustomText(100, font, Vector2f(screenSize.x / 2, screenSize.y / 10), Color::Black, String("MainMenu"));
		win.draw(titleText);
		for (Button& button : game.ButtonList)
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
		for (Button& button : game.ButtonList)
			button.DrawButton(win);
	}
	break;
	case WinMenu:
	{
		CustomText winText = CustomText(100, font, Vector2f(screenSize.x / 2, screenSize.y / 2), Color::Black, String("Win"));
		win.draw(winText);
		for (Button& button : game.ButtonList)
			button.DrawButton(win);
		break;
	}
	break;
	case LooseMenu:
	{
		CustomText looseText = CustomText(100, font, Vector2f(screenSize.x / 2, screenSize.y / 2), Color::Black, String("Loose"));
		win.draw(looseText);
		for (Button& button : game.ButtonList)
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
	//if (tank.currentShell < tank.maxShell && ((time - tank.lastShootingTime) > 0.5f || tank.lastShootingTime == 0))
	{
		tank.lastShootingTime = time;
		tank.currentShell++;
		game.AddShell(tank, pos, lightShell, explosionTexture);
	}
}

void		DrawCrosshair(RenderWindow& win)
{
	Vector2f	tankPosition = game.tankList[0].hull.getPosition();
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

	game.tankList[0].SetGunAngle(mouseWorldPos);
	win.draw(dot);
	win.draw(HorCross);
	win.draw(VerCross);
}

void		DrawElement(RenderWindow& win)
{
	for (Shell& shell : game.shellList)
		win.draw(shell.shell);
	for (Tank& tank : game.tankList)
	{
		tank.SetGunOnTank();
		win.draw(tank);
	}
	for (Wall& elemen : game.wallList)
		win.draw(elemen.wall);
}

void		World(RenderWindow& win, float time)
{
	if (gameState == GameState::Playing)
	{
		game.TankManager(time);
		game.ShellManager(time);
		if (game.tankList.empty() == false)
		{
			if (game.tankList[0].name == "Player")
			{
				DrawCrosshair(win);
				if (game.tankList.size() == 1)
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

void		LoadTexture()
{
	Texture* texture = new Texture();

	if (!Shader::isAvailable)
		printf("No shader available\n");
	if (!simpleShader->loadFromFile("res/simple.vert", "res/bloom.frag"))
		printf("unable to load shaders\n");

	if (!blueHull->loadFromFile("res/Tank/Blue/Hull.png"))
		printf("Unable to load blue hull\n");

	if (!blueGun->loadFromFile("res/Tank/Blue/Gun.png"))
		printf("Unable to load blue gun\n");

	if (!lightShell->loadFromFile("res/Shell/Light_Shell.png"))
		printf("Unable to load shell\n");

	/*if (!hull->loadFromFile("res/Tank/Brown/Hull.png"))
		printf("Unable to load brown hull\n");
	brownTankTexture.push_back(hull);

	gun = new Texture();
	if (!gun->loadFromFile("res/Tank/Brown/Gun.png"))
		printf("Unable to load brown gun\n");
	brownTankTexture.push_back(gun);*/


	///

	texture = new Texture();
	if (!texture->loadFromFile("res/Tank/Blue/Hull.png"))
		printf("Unable to load blue hull\n");
	else
		blueTankTexture.push_back(texture);

	texture = new Texture();
	if (!texture->loadFromFile("res/Tank/Blue/Gun.png"))
		printf("Unable to load blue gun\n");
	else
		blueTankTexture.push_back(texture);


	for (int i = 1; i != 9; i++)
	{
		texture = new Texture();
		if (!texture->loadFromFile("res/Explosion/Explosion" + to_string(i) + ".png"))
			printf("Unable to load Explosion%d\n", i);
		else
			explosionTexture.push_back(texture);
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

	settings.antialiasingLevel = 0;
	window.setMouseCursorVisible(true);
	window.setVerticalSyncEnabled(true);

	font->loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
	LoadTexture();

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
						game.ButtonList.clear();
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
					for (Button& button : game.ButtonList)
					{
						if (button.CheckButton(mouseWorldPos))
						{
							menuState = button.state;
							game.ButtonList.clear();
							switch (menuState)
							{
							case MainMenu:
								gameState = GameState::Start;
								game.ClearData();
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
								game.ClearData();
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
			game.AddWall("WallMap", Vector2f(0, 0), Vector2f(screenSize.x, WallSize));
			game.AddWall("WallMap", Vector2f(0, 0), Vector2f(WallSize, screenSize.y));
			game.AddWall("WallMap", Vector2f(screenSize.x - WallSize, 0), Vector2f(WallSize, screenSize.y));
			game.AddWall("WallMap", Vector2f(0, screenSize.y - WallSize), Vector2f(screenSize.x, WallSize));
			game.AddWall("Obstacle verticale", Vector2f(screenSize.x / 2, screenSize.y / 2), Vector2f(50, 100));
			game.AddWall("Obstacle horizontale", Vector2f(screenSize.x / 2 - 100, screenSize.y / 2 + 100), Vector2f(150, 50));
			game.AddWall("Obstacle carre", Vector2f(screenSize.x / 2 + 200, screenSize.y / 2), Vector2f(50, 50));

			game.AddTank("Player", Vector2f(screenSize.x / 2, 500), Color::Blue, simpleShader, blueHull, blueGun, explosionTexture);
			game.AddTank("Bot 1", Vector2f(300, 100), Color::Red, simpleShader, blueHull, blueGun, explosionTexture);
			game.AddTank("Bot 2", Vector2f(500, 100), Color::Red, simpleShader, blueHull, blueGun, explosionTexture);

			gameState = GameState::Pause;
			menuState = MenuState::PauseMenu;
			break;
		case Playing:
			if (isMouseCursorIsVisible == true) { isMouseCursorIsVisible = false; window.setMouseCursorVisible(false); }
			if (Keyboard::isKeyPressed(Keyboard::Right))game.tankList[0].MoveTank(MoveDirection::Right);
			if (Keyboard::isKeyPressed(Keyboard::Left))game.tankList[0].MoveTank(MoveDirection::Left);
			if (Keyboard::isKeyPressed(Keyboard::Up))game.tankList[0].MoveTank(MoveDirection::Up);
			if (Keyboard::isKeyPressed(Keyboard::Down))game.tankList[0].MoveTank(MoveDirection::Down);
			if (Mouse::isButtonPressed(Mouse::Left))AddShell(game.tankList[0], mouseWorldPos, frameStart.asSeconds());
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
		ShowDebug(window, fps);
		window.display();

		frameEnd = clock.getElapsedTime();
		lastFrame = frameStart;
	}
	return 0;
}