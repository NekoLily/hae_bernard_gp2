#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

using namespace sf;

class Button
{
public:
	Text			textButton;
	RectangleShape	shapeButton;
	Button(float _characterSize, String  _text , Vector2f _pos, Color _textcolor, Font _font, Color _buttoncolor)
	{
		textButton.setCharacterSize(_characterSize);
		textButton.setFont(_font);
		textButton.setPosition(_pos);
		textButton.setString(_text);
		std::cout << textButton.getString().toAnsiString() << std::endl;
		textButton.setFillColor(_textcolor);
		
		shapeButton.setSize(Vector2f(470, _characterSize));
		shapeButton.setPosition(_pos);
		shapeButton.setFillColor(_buttoncolor);
	}
};