#pragma once

enum GameState
{
	Start, Loading, Menu, Playing, Retry, Pause, End,
};

enum MenuState
{
	MainMenu, SinglePlayerMenu, MultiPlayerMenu, InGameMenu, PauseMenu, WinMenu, LooseMenu, Reload, Null,
};

enum MoveDirection
{
	Up, Left, Right, Down,
};