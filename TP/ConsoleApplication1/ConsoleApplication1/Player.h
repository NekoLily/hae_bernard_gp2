#pragma once
#include "Tank.h"
#include "Enum.h"
#include "Game.h"


class Player
{
public:
	PlayerController	playerController;
	PlayerInitState		playerInitState = PlayerInitState::Init;

	Player(Game* _game, PlayerController _playerController, Vector2f* _mouseWorldPos, int _indexController = -1)
	{
		game = _game;
		playerController = _playerController;
		indexController = _indexController;
		mouseWorldPos = _mouseWorldPos;
	};

	void	Update(float time)
	{
		switch (playerController)
		{
		case PlayerController::Keyboard:
			KeyboardController(time);
			break;
		case PlayerController::GamePad:
			GamePadController(time);
			break;
		default:
			break;
		}
	}

	void	SetTankToPlayer(Tank* _tank)
	{
		tank = _tank;
	}

	void	GamePadController(float time)
	{
		float LeftAxeX = sf::Joystick::getAxisPosition(indexController, sf::Joystick::X);
		float LeftAxeY = sf::Joystick::getAxisPosition(indexController, sf::Joystick::Y);
		//printf("X = %f | Y = %f\n", LeftAxeX, LeftAxeY);

		float RightAxeX = sf::Joystick::getAxisPosition(indexController, sf::Joystick::U);
		float RightAxeY = sf::Joystick::getAxisPosition(indexController, sf::Joystick::V);
		//printf("X = %f | Y = %f\n", RightAxeX, RightAxeY);


		float AxeTrigger = sf::Joystick::getAxisPosition(indexController, sf::Joystick::Z);
		//printf("axe = %f\n", AxeTrigger);

		//-----------------------------------------------------------------
		float MoveX = 0;
		float MoveY = 0;
		if (LeftAxeX <= -30)
			MoveX = -1;
		else if (LeftAxeX >= 30)
			MoveX = 1;
		if (LeftAxeY <= -25)
			MoveY = -1;
		else if (LeftAxeY >= 25)
			MoveY = 1;

		if (MoveY == -1)
			tank->MoveTank(MoveDirection::Up);
		else if (MoveY == 1)
			tank->MoveTank(MoveDirection::Down);
		else
			tank->MoveTank(Idle);

		if (MoveX == -1)
			tank->MoveTank(MoveDirection::RotateLeft);
		else if (MoveX == 1)
			tank->MoveTank(MoveDirection::RotateRight);
		//---------------------------------------------------------------
		Vector2f tankPos = tank->gun.getPosition();;
		Vector2f targetDirection = Vector2f(tankPos.x + RightAxeX, tankPos.y + RightAxeY);
		if (RightAxeX <= -75 || RightAxeX >= 75 || RightAxeY <= -75 || RightAxeY >= 75)
			tank->SetGunAngle(targetDirection);
		//---------------------------------------------------------------
		if (AxeTrigger <= -90)
			game->AddShell(*tank, targetDirection, time);

	}

	void	KeyboardController(float time)
	{
		if (Keyboard::isKeyPressed(Keyboard::Right))tank->MoveTank(MoveDirection::RotateRight);
		if (Keyboard::isKeyPressed(Keyboard::Left))tank->MoveTank(MoveDirection::RotateLeft);
		if (Keyboard::isKeyPressed(Keyboard::Up))tank->MoveTank(MoveDirection::Up);
		else if (Keyboard::isKeyPressed(Keyboard::Down))tank->MoveTank(MoveDirection::Down);
		else
			tank->MoveTank(MoveDirection::Idle);
		if (Mouse::isButtonPressed(Mouse::Left)&& tank->tankState == TankState::TankAlive)game->AddShell(*tank, *mouseWorldPos, time);
	}

	void	DrawCrosshair(RenderWindow& win)
	{
		Vector2f	tankPosition = tank->hull.getPosition();
		float		xDistance = tankPosition.x - mouseWorldPos->x;
		float		yDistance = tankPosition.y - mouseWorldPos->y;
		float		Distance = sqrt(xDistance * xDistance + yDistance * yDistance);

		float		xDirection = -xDistance / Distance;
		float		yDirection = -yDistance / Distance;

		CircleShape		dot(5);
		RectangleShape	HorCross(Vector2f(10, 2));
		RectangleShape	VerCross(Vector2f(2, 10));

		HorCross.setOrigin(10 / 2, 2 / 2);
		HorCross.setFillColor(Color::Red);
		HorCross.setPosition(*mouseWorldPos);

		VerCross.setOrigin(2 / 2, 10 / 2);
		VerCross.setFillColor(Color::Red);
		VerCross.setPosition(*mouseWorldPos);

		dot.setOrigin(5, 5);
		dot.setFillColor(Color::Transparent);
		dot.setOutlineThickness(1);
		dot.setOutlineColor(Color::Red);
		dot.setPosition(*mouseWorldPos);

		tank->SetGunAngle(*mouseWorldPos);
		win.draw(dot);
		win.draw(HorCross);
		win.draw(VerCross);
	}

private:
	int					indexController;
	Tank* tank;
	Game*				game;
	Vector2f			*mouseWorldPos;
};
