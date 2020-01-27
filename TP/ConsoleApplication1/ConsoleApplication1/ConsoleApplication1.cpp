#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream> 
#include <list>
#include <iterator> 
#include <string>
#include "Game.h"
#include "Player.h"
#include "Tank.h"
#include "Wall.h"
#include "Shell.h"
#include "UI.h"

using namespace sf;
using namespace std;

int						fps;
Game					game;
Vector2f				screenSize(1600, 900);

static Font* font = new Font();
static Shader* simpleShader = new Shader();

float					WallSize = 50;
float					offSetCrosshairRange = 70;

Vector2f				mouseWorldPos;
GameState				gameState;

vector<Texture*>		iconTextureList;

bool					isPause = false;

std::vector<Player>		playerList;
int						playerIndex = 0;
bool					ControllerAlreadyInit[9];
bool					playerAlreadyInit[3];

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
	if (!Shader::isAvailable())
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

	for (int i = 0; i <= 2; i++)
	{
		texture = new Texture();
		if (!texture->loadFromFile("res/Misc/" + to_string(i) + ".png"))
			printf("Unable to load misc %d\n", i);
		else
			iconTextureList.push_back(texture);
	}

	for (int i = 0; i <= 3; i++)
	{
		texture = new Texture();
		if (!texture->loadFromFile("res/Tank/" + to_string(i) + "-0.png"))
			printf("Unable to load tank '%d-0'\n", i);
		else
			game.tankTexture.push_back(texture);

		texture = new Texture();
		if (!texture->loadFromFile("res/Tank/" + to_string(i) + "-1.png"))
			printf("Unable to load hull '%d-0'\n", i);
		else
			game.tankTexture.push_back(texture);
	}

	for (int i = 0; i <= 8; i++)
	{
		texture = new Texture();
		if (!texture->loadFromFile("res/Explosion/" + to_string(i) + ".png"))
			printf("Unable to load Explosion%d\n", i);
		else
			game.explosionTexture.push_back(texture);
	}

	for (int i = 0; i <= 3; i++)
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
		game.AddTank("Player 1", TankTag::PlayerTank, Vector2f(screenSize.x / 3, 500), simpleShader);
		game.AddTank("Bot 1", TankTag::BotTank, Vector2f(300, 100), simpleShader);
		game.AddTank("Bot 2", TankTag::BotTank, Vector2f(500, 100), simpleShader);
		break;
	case GameMode::Versus:

		game.AddTank("Player 1", TankTag::PlayerTank, Vector2f(screenSize.x / 9, 500), simpleShader);
		game.AddTank("Player 2", TankTag::PlayerTank, Vector2f(screenSize.x - 100, 100), simpleShader);
		if (playerList.size() >= 3)
			game.AddTank("Player 3", TankTag::PlayerTank, Vector2f(100, 100), simpleShader);
		if (playerList.size() >= 4)
			game.AddTank("Player 4", TankTag::PlayerTank, Vector2f(screenSize.x - 100, 500), simpleShader);
		break;
	default:
		break;
	}
}

int			CheckNumberOfGamepadConnected()
{
	int nb = 0;
	for (int i = 0; i != 8; i++)
		if (sf::Joystick::isConnected(i))
			nb++;
	return nb;
}

void		PlayerControllerInit(UI* ui)
{
	for (int i = 0; i != 8; i++)
	{
		if (Joystick::isConnected(i))
		{
			if (Joystick::isButtonPressed(i, 7) && ControllerAlreadyInit[i] == false)
			{
				if (playerAlreadyInit[playerIndex] == false)
				{
					playerList.push_back(Player(&game, PlayerController::GamePad, &mouseWorldPos, i));
					playerAlreadyInit[playerIndex] = true;
					ui->AddControllerIcon(iconTextureList[1], playerIndex,i);
					ControllerAlreadyInit[i] = true;
					playerIndex++;
				}
			}
		}
	}

	if (Keyboard::isKeyPressed(Keyboard::Space))
	{
		for (int i = 0; i <= 3; i++)
		{
			if (playerAlreadyInit[i] == false)//&& ControllerAlreadyInit[9] == false
			{
				playerList.push_back(Player(&game, PlayerController::Keyboard, &mouseWorldPos));
				playerAlreadyInit[playerIndex] = true;
				ui->AddControllerIcon(iconTextureList[0], playerIndex);
				ControllerAlreadyInit[9] = true;
				playerIndex++;
			}
		}
	}
}

void		SetPlayerControllerData()
{
	for (int i = 0; i < playerIndex; i++)
	{
		playerList[i].SetTankToPlayer(&game.tankList[i]);
	}
}

void		ResetPlayerController()
{
	playerIndex = 0;
	for (int i = 0; i < 3; i++)
		playerAlreadyInit[i] = false;
	playerList.clear();
}

void		World(RenderWindow& win, UI& ui, float time)
{
	if (gameState == GameState::InGame)
	{
		game.TankManager(time);
		game.ShellManager(time);
		game.FlashManager(time);
		switch (game.GameManager())
		{
		case InGameState::PlayerWin:
			gameState = GameState::GameEnd;
			ui.screen = Screen::WinMenu;
			break;
		case InGameState::PlayerLoose:
			gameState = GameState::GameEnd;
			ui.screen = Screen::LooseMenu;
			break;
		case InGameState::VersusEnd:
			gameState = GameState::GameEnd;
			ui.screen = Screen::StartButton;
			break;
		case InGameState::Draw:
			gameState = GameState::GameEnd;
			ui.screen = Screen::DrawMenu;
			break;
		case InGameState::Playing:
			for (Player& player : playerList)
				if (player.playerController == PlayerController::Keyboard)
					player.DrawCrosshair(win);
			break;
		default:
			break;
		}
	}
}

int			main()
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

	UI			ui(font, screenSize, iconTextureList);

	while (window.isOpen())
	{
		window.clear(Color(212, 192, 171, 255));
		frameStart = clock.getElapsedTime();
		fps = 1.0f / (frameStart - lastFrame).asSeconds();
		mouseWorldPos = window.mapPixelToCoords(Mouse::getPosition(window));

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::LostFocus && gameState == GameState::InGame)
			{
				isPause = true;
				gameState = GameState::Pause;
				ui.screen = Screen::PauseMenu;
				ui.InitMenu();
			}
			if (event.type == Event::KeyPressed)
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
						ui.screen = Screen::PauseMenu;
						ui.InitMenu();
					}
					break;
				case Pause:
					if (event.key.code == Keyboard::Space)
					{
						isPause = true;
						gameState = GameState::InGame;
						ui.FreeUIData();
					}
					break;
				default:
					break;
				}
			}
			if (event.type == Event::MouseButtonPressed)
				if (gameState == GameState::Menu || gameState == GameState::GameEnd || gameState == GameState::Pause)
					if (event.mouseButton.button == Mouse::Left)
						ui.CheckButton(game, gameState, mouseWorldPos);
			if (event.type == Event::Closed)
				window.close();
		}

		switch (gameState)
		{
		case Start:
			ResetPlayerController();
			ui.InitMenu();
			gameState = GameState::Menu;
			break;
		case Menu:
			if (ui.screen == Screen::MultiPlayerMenu)
				PlayerControllerInit(&ui);
			ui.Draw(window);
			break;
		case Loading:
			LoadMap();
			SetPlayerControllerData();
			gameState = GameState::Pause;
			ui.screen = Screen::PauseMenu;
			break;
		case InGame:
			if (isMouseCursorIsVisible == true) { isMouseCursorIsVisible = false; window.setMouseCursorVisible(false); }
			for (Player& player : playerList)
				player.Update(frameStart.asSeconds());
			game.DrawElement(window);
			World(window, ui, frameStart.asSeconds());
			break;
		case Pause:
			ui.InitMenu();
			game.DrawElement(window);
			ui.Draw(window);
			if (isMouseCursorIsVisible == false) { isMouseCursorIsVisible = true; window.setMouseCursorVisible(true); }
			break;
		case GameEnd:
			ui.InitMenu(game.nameVersusWinner);
			game.DrawElement(window);
			ui.Draw(window);
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