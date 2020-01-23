#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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

int						fps;
Game					game;
Vector2f				screenSize(800, 600);

static Font* font = new Font();
static Shader* simpleShader = new Shader();

float					WallSize = 50;
float					offSetCrosshairRange = 70;

Vector2f				mouseWorldPos;

GameState				gameState;
MenuTag					menuTag;
CustomText				titleText;
std::vector<Button>		ButtonList;

bool				isPause = false;

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

void		LoadShader()
{
	if (!Shader::isAvailable)
		printf("No shader available\n");
	if (!simpleShader->loadFromFile("res/simple.vert", "res/bloom.frag"))
		printf("unable to load shaders\n");
}

void		LoadTexture()
{
	Texture* texture = new Texture();
	if (!texture->loadFromFile("res/Shell/Light_Shell.png"))
		printf("Unable to load shell\n");
	else
		game.shellTexture = texture;
	for (int i = 0; i != 2; i++)
	{
		texture = new Texture();
		if (!texture->loadFromFile("res/Tank/Blue/" + to_string(i) + ".png"))
			printf("Unable to load blue %d\n", i);
		else
			game.blueTankTexture.push_back(texture);
	}

	for (int i = 0; i != 2; i++)
	{
		texture = new Texture();
		if (!texture->loadFromFile("res/Tank/Brown/" + to_string(i) + ".png"))
			printf("Unable to load brown %d\n", i);
		else
			game.brownTankTexture.push_back(texture);
	}

	for (int i = 0; i != 9; i++)
	{
		texture = new Texture();
		if (!texture->loadFromFile("res/Explosion/" + to_string(i) + ".png"))
			printf("Unable to load Explosion%d\n", i);
		else
			game.explosionTexture.push_back(texture);
	}

	for (int i = 0; i != 4; i++)
	{
		texture = new Texture();
		if (!texture->loadFromFile("res/Flash/" + to_string(i) + ".png"))
			printf("Unable to load flash %d\n", i);
		else
			game.flashTexture.push_back(texture);
	}

}

void		LoadSound()
{
	SoundBuffer* soundBuffer = new SoundBuffer();
	if (!soundBuffer->loadFromFile("res/Audio/ExplosionTank.wav"))
		printf("unable to load ExplosionTank\n");
	else
		game.soundBufferVec.push_back(soundBuffer);

	soundBuffer = new SoundBuffer();
	if (!soundBuffer->loadFromFile("res/Audio/ExplosionShell.wav"))
		printf("unable to load ExplosionShell\n");
	else
		game.soundBufferVec.push_back(soundBuffer);
}

void		InitMenu()
{
	if (menuTag == MenuTag::MainMenu)
	{
		Button singlePlayerButton = Button(470, 70, String("Single Player"), Vector2f(screenSize.x - screenSize.x / 1.3, screenSize.y / 3), Color::Black, font, Color::Green, MenuTag::SinglePlayerMenu);
		ButtonList.push_back(singlePlayerButton);
		Button MultiPlayerButton = Button(470, 70, String("Versus"), Vector2f(screenSize.x - screenSize.x / 1.3, screenSize.y / 2), Color::Black, font, Color::Green, MenuTag::MultiPlayerMenu);
		ButtonList.push_back(MultiPlayerButton);
	}
	else
	{
		Button quitButton(70, 30, String("Quit"), Vector2f(screenSize.x / 2 - screenSize.x / 5, screenSize.y / 1.5), Color::Black, font, Color::Green, MenuTag::MainMenu);
		ButtonList.push_back(quitButton);
		Button retryButton(70, 30, String("Retry"), Vector2f(screenSize.x / 2 + screenSize.x / 5, screenSize.y / 1.5), Color::Black, font, Color::Green, MenuTag::Reload);
		ButtonList.push_back(retryButton);
	}
	switch (menuTag)
	{
	case MainMenu:
		titleText = CustomText(100, font, Vector2f(screenSize.x / 2, screenSize.y / 10), Color::Black, String("MainMenu"));
		break;
	case SinglePlayerMenu:
		break;
	case MultiPlayerMenu:
		break;
	case InGameMenu:
		break;
	case PauseMenu:
		titleText = CustomText(100, font, Vector2f(screenSize.x / 2, screenSize.y / 2), Color::Black, String("Pause"));
		break;
	case WinMenu:
		titleText = CustomText(100, font, Vector2f(screenSize.x / 2, screenSize.y / 2), Color::Black, String("Win"));
		break;
	case LooseMenu:
		titleText = CustomText(100, font, Vector2f(screenSize.x / 2, screenSize.y / 2), Color::Black, String("Loose"));
		break;
	case Player1WinMenu:
		titleText = CustomText(100, font, Vector2f(screenSize.x / 2, screenSize.y / 2), Color::Black, String("Player 1 win !"));
		break;
	case Player2WinMenu:
		titleText = CustomText(100, font, Vector2f(screenSize.x / 2, screenSize.y / 2), Color::Black, String("Player 2 win !"));
		break;
	case DrawMenu:
		titleText = CustomText(100, font, Vector2f(screenSize.x / 2, screenSize.y / 2), Color::Black, String("Draw !"));
		break;
	case Reload:
		break;
	case Null:
		break;
	default:
		break;
	}
}

void		DrawUI(RenderWindow& win)
{
	win.draw(titleText);
	for (Button& button : ButtonList)
		button.DrawButton(win);
}

void		CheckButton()
{
	for (Button& button : ButtonList)
	{
		if (button.CheckButton(mouseWorldPos))
		{
			menuTag = button.state;
			ButtonList.clear();
			switch (menuTag)
			{
			case MainMenu:
				gameState = GameState::Start;
				game.ResetData();
				break;
			case SinglePlayerMenu:
				game.gameMode = GameMode::Solo;
				gameState = GameState::Loading;
				break;
			case MultiPlayerMenu:
				game.gameMode = GameMode::Versus;
				gameState = GameState::Loading;
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
				game.ResetData();
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
	HorCross.setPosition(mouseWorldPos);

	VerCross.setOrigin(2 / 2, 10 / 2);
	VerCross.setFillColor(Color::Red);
	VerCross.setPosition(mouseWorldPos);

	dot.setOrigin(5, 5);
	dot.setFillColor(Color::Transparent);
	dot.setOutlineThickness(1);
	dot.setOutlineColor(Color::Red);
	dot.setPosition(mouseWorldPos);

	game.tankList[0].SetGunAngle(mouseWorldPos);
	win.draw(dot);
	win.draw(HorCross);
	win.draw(VerCross);
}

void		LoadMap()
{
	game.AddWall("WallMapUP", Vector2f(0, 0), Vector2f(screenSize.x, WallSize));
	game.AddWall("WallMapLeft", Vector2f(0, 0), Vector2f(WallSize, screenSize.y));
	game.AddWall("WallMapRight", Vector2f(screenSize.x - WallSize, 0), Vector2f(WallSize, screenSize.y));
	game.AddWall("WallMapBot", Vector2f(0, screenSize.y - WallSize), Vector2f(screenSize.x, WallSize));

	switch (game.gameMode)
	{
	case GameMode::Solo:
		game.AddWall("Obstacle verticale", Vector2f(screenSize.x / 2, screenSize.y / 2), Vector2f(50, 100));
		game.AddWall("Obstacle horizontale", Vector2f(screenSize.x / 2 - 100, screenSize.y / 2 + 100), Vector2f(150, 50));
		game.AddWall("Obstacle carre", Vector2f(screenSize.x / 2 + 200, screenSize.y / 2), Vector2f(50, 50));
		game.AddTank("Player1", TankTag::Player, Vector2f(screenSize.x / 2, 500), Color::Blue, simpleShader);
		game.AddTank("Bot 1", TankTag::Bot, Vector2f(300, 100), Color::Red, simpleShader);
		game.AddTank("Bot 2", TankTag::Bot, Vector2f(500, 100), Color::Red, simpleShader);
		break;
	case GameMode::Versus:
		game.AddTank("Player1", TankTag::Player, Vector2f(screenSize.x / 9, 500), Color::Blue, simpleShader);
		game.AddTank("Player2", TankTag::Player, Vector2f(screenSize.x - 100, 100), Color::Red, simpleShader);
		break;
	default:
		break;
	}

}

void		World(RenderWindow& win, float time)
{
	if (gameState == GameState::InGame)
	{
		game.TankManager(time);
		game.ShellManager(time);
		game.FlashManager(time);
		switch (game.GameManager())
		{
		case InGameState::PlayerWin:
			gameState = GameState::End;
			menuTag = MenuTag::WinMenu;
			break;
		case InGameState::PlayerLoose:
			gameState = GameState::End;
			menuTag = MenuTag::LooseMenu;
			break;
		case InGameState::Player1Win:
			gameState = GameState::End;
			menuTag = MenuTag::Player1WinMenu;
			break;
		case InGameState::Player2Win:
			gameState = GameState::End;
			menuTag = MenuTag::Player2WinMenu;
			break;
		case InGameState::Draw:
			gameState = GameState::End;
			menuTag = MenuTag::DrawMenu;
			break;
		case InGameState::Playing:
			DrawCrosshair(win);
			break;
		default:
			break;
		}
	}
}

void		CheckGameState(Event event)
{
	switch (gameState)
	{
	case Menu:
		break;
	case InGame:
		if (event.key.code == Keyboard::Space)
		{
			isPause = true;
			gameState = GameState::Pause;
			menuTag = MenuTag::PauseMenu;
			InitMenu();
		}
		else if (event.type == Event::LostFocus)
		{
			isPause = true;
			gameState = GameState::Pause;
			menuTag = MenuTag::PauseMenu;
			InitMenu();
		}
		break;
	case Pause:
		if (event.key.code == Keyboard::Space)
		{
			isPause = true;
			gameState = GameState::InGame;
			ButtonList.clear();
		}
		break;
	default:
		break;
	}
}

int	main()
{
	ContextSettings		settings;
	RenderWindow		window(VideoMode(screenSize.x, screenSize.y), "World of tank low cost");
	Clock				clock;
	Time				time = clock.getElapsedTime();
	Time				frameStart;
	Time				frameEnd;
	Time				lastFrame;

	bool				isMouseCursorIsVisible = true;

	settings.antialiasingLevel = 2;
	window.setMouseCursorVisible(true);
	window.setVerticalSyncEnabled(true);

	font->loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
	LoadTexture();
	LoadSound();
	gameState = GameState::Start;
	menuTag = MenuTag::MainMenu;

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
				CheckGameState(event);
			}
			if (event.type == Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == Mouse::Left)
				{
					//CircleShape mouseClick(5);
					//mouseClick.setOrigin(Vector2f(mouseClick.getRadius(), mouseClick.getRadius()));
					//mouseClick.setPosition(mouseWorldPos);
					CheckButton();
				}
			}
			if (event.type == Event::Closed)
				window.close();
		}

		mouseWorldPos = window.mapPixelToCoords(Mouse::getPosition(window));

		switch (gameState)
		{
		case Start:
			InitMenu();
			gameState = GameState::Menu;
			break;
		case Menu:
			DrawUI(window);
			break;
		case Loading:
			LoadMap();
			gameState = GameState::Pause;
			menuTag = MenuTag::PauseMenu;
			break;
		case InGame:
			if (isMouseCursorIsVisible == true) { isMouseCursorIsVisible = false; window.setMouseCursorVisible(false); }
			if (Keyboard::isKeyPressed(Keyboard::Right))game.tankList[0].MoveTank(MoveDirection::Right);
			if (Keyboard::isKeyPressed(Keyboard::Left))game.tankList[0].MoveTank(MoveDirection::Left);
			if (Keyboard::isKeyPressed(Keyboard::Up))game.tankList[0].MoveTank(MoveDirection::Up);
			if (Keyboard::isKeyPressed(Keyboard::Down))game.tankList[0].MoveTank(MoveDirection::Down);
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				game.AddShell(game.tankList[0], mouseWorldPos, frameStart.asSeconds());
				//game.AddShell(game.tankList[1], mouseWorldPos, lightShell, explosionTexture, frameStart.asSeconds());
			}

			game.DrawElement(window);
			World(window, frameStart.asSeconds());
			break;
		case Pause:
			InitMenu();
			game.DrawElement(window);
			DrawUI(window);
			if (isMouseCursorIsVisible == false) { isMouseCursorIsVisible = true; window.setMouseCursorVisible(true); }
			break;
		case End:
			InitMenu();
			game.DrawElement(window);
			DrawUI(window);
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