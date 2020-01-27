#pragma once

enum GameState
{
	Start, Loading, Menu, InGame, Retry, Pause, GameEnd,
};

enum InGameState
{
	Playing, PlayerWin, PlayerLoose, VersusEnd, Draw,
};

enum Screen
{
	MainMenu, SinglePlayerMenu, MultiPlayerMenu,  PauseMenu, WinMenu, LooseMenu, StartButton, DrawMenu, Reload, Null,
};

enum MoveDirection
{
	Up, RotateLeft, RotateRight, Down, Idle,
};

enum TankState
{
	TankAlive, TankExplode, TankDestroy, TankDead,
};

enum TankTag
{
	PlayerTank, BotTank,
};

enum ShellState
{
	Shellinit, ShellMoving, ShellExplode, ShellDestroy,
};

enum FlashState
{
	FlashPlaying, FlashFisnih
};

enum GameMode
{
	Solo, Versus,
};

enum PlayerController
{
	Keyboard, GamePad,
};

enum PlayerInitState
{
	Init, Select, Ready
};

enum PlayerInitControlState
{
	MovingLeft, MovingRight, ControlIdle,
};