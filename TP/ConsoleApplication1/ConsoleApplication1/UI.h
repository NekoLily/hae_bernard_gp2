#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "Game.h"
#include "Enum.h"

using namespace sf;
using namespace std;

class CustomText : public Text
{
public:
	CustomText(int _characterSize = 0, Font* _font = nullptr, Vector2f _pos = Vector2f(0, 0), Color _fillColor = Color(), String _string = "")
	{
		this->setCharacterSize(_characterSize);
		this->setFont(*_font);
		this->setFillColor(_fillColor);
		this->setString(_string);
		FloatRect textBox = this->getLocalBounds();
		this->setOrigin(Vector2f(textBox.left + textBox.width / 2, textBox.top + textBox.height / 2));
		this->setPosition(_pos);
	}
};

class Button
{
public:
	RectangleShape	shapeButton;
	FloatRect		buttonBox;
	Screen		state;
	Text			text;

	Button(float _buttonWidth, float _characterSize, String  _text, Vector2f _pos, Color _textcolor, Font* _font, Color _buttoncolor, Screen _state)
	{
		shapeButton.setSize(Vector2f(_buttonWidth, _characterSize));
		shapeButton.setPosition(_pos);
		shapeButton.setFillColor(_buttoncolor);
		shapeButton.setOrigin(Vector2f(_buttonWidth / 2, _characterSize / 2));

		text.setCharacterSize(_characterSize);
		text.setFont(*_font);
		text.setString(_text);
		FloatRect textBox = text.getLocalBounds();
		text.setOrigin(Vector2f(textBox.left + textBox.width / 2, textBox.top + textBox.height / 2));
		buttonBox = shapeButton.getGlobalBounds();
		text.setPosition(Vector2f(buttonBox.left + buttonBox.width / 2, buttonBox.top + buttonBox.height / 2));
		text.setFillColor(_textcolor);
		state = _state;
	}

	void	DrawButton(RenderWindow& win)
	{
		win.draw(shapeButton);
		win.draw(text);
	}

	bool	CheckButton(Vector2f _mousePos)
	{
		bool	isThisButton = false;
		if (_mousePos.x >= buttonBox.left && _mousePos.x <= buttonBox.left + buttonBox.width && _mousePos.y >= buttonBox.top && _mousePos.y < buttonBox.top + buttonBox.height)
			isThisButton = true;
		return isThisButton;
	}
};

class UI
{
public:
	Screen					screen;

	UI(Font* _font, Vector2f _screenSize, vector<Texture*> _iconTextureList)
	{
		font = _font;
		screenSize = _screenSize;
		iconTextureList = _iconTextureList;
		screen = Screen::MainMenu;
	};

	void		InitMenu(String nameVersusWinner = "")
	{
		FreeUIData();
		CustomText* text = nullptr;
		if (screen == Screen::MainMenu)
		{
			Button singlePlayerButton = Button(470, 70, String("Single Player"), Vector2f(screenSize.x - screenSize.x / 2, screenSize.y / 3), Color::Black, font, Color::Green, Screen::SinglePlayerMenu);
			buttonList.push_back(singlePlayerButton);
			Button MultiPlayerButton = Button(470, 70, String("Versus"), Vector2f(screenSize.x - screenSize.x / 2, screenSize.y / 2), Color::Black, font, Color::Green, Screen::MultiPlayerMenu);
			buttonList.push_back(MultiPlayerButton);
		}
		else if (screen == Screen::MultiPlayerMenu)
		{
			Button MultiPlayerButton = Button(470, 70, String("Start !"), Vector2f(screenSize.x - screenSize.x / 2, screenSize.y - 100), Color::Black, font, Color::Green, Screen::StartButton);
			buttonList.push_back(MultiPlayerButton);
		}
		else
		{
			Button quitButton(70, 30, String("Quit"), Vector2f(screenSize.x / 2 - screenSize.x / 5, screenSize.y / 1.5), Color::Black, font, Color::Green, Screen::MainMenu);
			buttonList.push_back(quitButton);
			Button retryButton(70, 30, String("Retry"), Vector2f(screenSize.x / 2 + screenSize.x / 5, screenSize.y / 1.5), Color::Black, font, Color::Green, Screen::Reload);
			buttonList.push_back(retryButton);
		}
		switch (screen)
		{
		case MainMenu:
			text = new  CustomText(100, font, Vector2f(screenSize.x / 2, screenSize.y / 10), Color::Black, String("World of tank low cost"));
			textList.push_back(text);
			break;
		case SinglePlayerMenu:
			break;
		case MultiPlayerMenu:
		{
			text = new  CustomText(100, font, Vector2f(screenSize.x / 2, screenSize.y / 10), Color::Black, String("Versus Menu"));
			textList.push_back(text);
			text = new CustomText(50, font, Vector2f(screenSize.x / 2 - screenSize.x / 3, screenSize.y / 4), Color::Black, String("Player 1"));
			textList.push_back(text);
			text = new CustomText(50, font, Vector2f(screenSize.x / 2 - screenSize.x / 10, screenSize.y / 4), Color::Black, String("Player 2"));
			textList.push_back(text);
			text = new CustomText(50, font, Vector2f(screenSize.x / 2 + screenSize.x / 10, screenSize.y / 4), Color::Black, String("Player 3"));
			textList.push_back(text);
			text = new CustomText(50, font, Vector2f(screenSize.x / 2 + screenSize.x / 3, screenSize.y / 4), Color::Black, String("Player 4"));
			textList.push_back(text);
			text = new CustomText(50, font, Vector2f(screenSize.x / 2, screenSize.y - screenSize.y / 10), Color::Black, String("Press start or space to join"));
			textList.push_back(text);
		}
		break;
		case PauseMenu:
			text = new  CustomText(100, font, Vector2f(screenSize.x / 2, screenSize.y / 2), Color::Black, String("Pause"));
			textList.push_back(text);
			break;
		case WinMenu:
			text = new  CustomText(100, font, Vector2f(screenSize.x / 2, screenSize.y / 2), Color::Black, String("Win"));
			textList.push_back(text);
			break;
		case LooseMenu:
			text = new  CustomText(100, font, Vector2f(screenSize.x / 2, screenSize.y / 2), Color::Black, String("Loose"));
			textList.push_back(text);
			break;
		case StartButton:
			text = new  CustomText(100, font, Vector2f(screenSize.x / 2, screenSize.y / 2), Color::Black, String(nameVersusWinner + " win !"));
			textList.push_back(text);
			break;
		case DrawMenu:
			text = new  CustomText(100, font, Vector2f(screenSize.x / 2, screenSize.y / 2), Color::Black, String("Draw !"));
			textList.push_back(text);
			break;
		case Reload:
			break;
		case Null:
			break;
		default:
			break;
		}
	}

	void		AddControllerIcon(Texture* iconTexture, int indexPlayer, int id = -1)
	{
		Sprite* spr = new Sprite();
		switch (indexPlayer)
		{
		case 0:
			spr->setScale(0.15, 0.15);
			spr->setPosition(Vector2f(screenSize.x / 2 - screenSize.x / 3, screenSize.y / 4 + screenSize.y / 15));
			spr->setTexture(*iconTexture);
			spr->setOrigin(Vector2f(spr->getTexture()->getSize().x / 2, spr->getTexture()->getSize().y / 2));
			break;
		case 1:
			spr->setScale(0.15, 0.15);
			spr->setPosition(Vector2f(screenSize.x / 2 - screenSize.x / 10, screenSize.y / 4 + screenSize.y / 15));
			spr->setTexture(*iconTexture);
			spr->setOrigin(Vector2f(spr->getTexture()->getSize().x / 2, spr->getTexture()->getSize().y / 2));
			break;
		case 2:
			spr->setScale(0.15, 0.15);
			spr->setPosition(Vector2f(screenSize.x / 2 + screenSize.x / 10, screenSize.y / 4 + screenSize.y / 15));
			spr->setTexture(*iconTexture);
			spr->setOrigin(Vector2f(spr->getTexture()->getSize().x / 2, spr->getTexture()->getSize().y / 2));
			break;
		case 3:
			spr->setScale(0.15, 0.15);
			spr->setPosition(Vector2f(screenSize.x / 2 + screenSize.x / 3, screenSize.y / 4+ screenSize.y / 15));
			spr->setTexture(*iconTexture);
			spr->setOrigin(Vector2f(spr->getTexture()->getSize().x / 2, spr->getTexture()->getSize().y / 2));
			break;
		default:
			break;
		}
		
		iconSpriteList.push_back(spr);
		if (id != -1)
		{
			CustomText* text = new  CustomText(50, font, Vector2f(screenSize.x / 2 - screenSize.x / 3 + screenSize.x / 20, screenSize.y / 4 + screenSize.y / 15), Color::Black, String("n°" + to_string(id + 1)));
			textList.push_back(text);
		}
	}

	void		Draw(RenderWindow& win)
	{
		for (CustomText* text : textList)
			win.draw(*text);
		for (Button& button : buttonList)
			button.DrawButton(win);
		for (Sprite* sprite : iconSpriteList)
			win.draw(*sprite);
	}

	void		CheckButton(Game& game, GameState& gameState, Vector2f mouseWorldPos)
	{
		for (Button& button : buttonList)
		{
			if (button.CheckButton(mouseWorldPos))
			{
				screen = button.state;
				switch (screen)
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
					InitMenu();
					break;
				case StartButton:
					game.gameMode = GameMode::Versus;
					gameState = GameState::Loading;
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

	void		FreeUIData()
	{
		for (vector<CustomText*>::iterator it = textList.begin(); it != textList.end(); ++it)
			delete* it;
		for (vector<Sprite*>::iterator it = iconSpriteList.begin(); it != iconSpriteList.end(); ++it)
			delete* it;
		textList.clear();
		buttonList.clear();
		iconSpriteList.clear();
	}

private:
	Font* font;
	Vector2f				screenSize;

	vector<Texture*>		iconTextureList;
	vector<CustomText*>		textList;
	vector<Button>			buttonList;
	vector<Sprite*>			iconSpriteList;
};