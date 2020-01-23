#pragma once

enum GameState
{
	Start, Loading, Menu, InGame, Retry, Pause, End,
};

enum InGameState
{
	Playing, PlayerWin, PlayerLoose, Player1Win, Player2Win, Draw,
};

enum MenuTag
{
	MainMenu, SinglePlayerMenu, MultiPlayerMenu, InGameMenu, PauseMenu, WinMenu, LooseMenu, Player1WinMenu, Player2WinMenu, DrawMenu, Reload, Null,
};

enum MoveDirection
{
	Up, Left, Right, Down,
};

enum TankState
{
	TankAlive, TankExplode, TankDestroy, TankDead,
};

enum TankTag
{
	Player, Bot,
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