#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "Enum.h"

using namespace sf;

class CustomText : public Text
{
public:
	CustomText(int _characterSize, Font * _font, Vector2f _pos, Color _fillColor, String _string)
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
	MenuState		state;
	Text			text;

	Button(float _buttonWidth, float _characterSize, String  _text, Vector2f _pos, Color _textcolor, Font* _font, Color _buttoncolor, MenuState _state)
	{
		shapeButton.setSize(Vector2f(_buttonWidth, _characterSize));
		shapeButton.setPosition(_pos);
		shapeButton.setFillColor(_buttoncolor);

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