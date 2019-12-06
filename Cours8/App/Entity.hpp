#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;

enum PlayerState
{
	Idle, Moving, Jumping, Falling
};

class Entity {
public:
	
	sf::Shape * spr;
	sf::Font *font;
	PlayerState playerState = PlayerState::Idle;
	Entity( sf::Shape * spr ) {
		this->spr = spr;
	}

	int CELL_WIDTH = 16;
	int CELL_HEIGHT = 16;

	float cx = 0;
	float cy = 0;

	float rx = 0.5;
	float ry = 0.5;

	float dx = 0;
	float dy = 0;

	int pixelx = 0;
	int pixely = 0;

	bool willCollide(int x, int y)
		{
		float screenWidth = 1280 / 16;
		float screenHeight = 720 / 16;
		if (x < 1) return true;
		else if (x > screenWidth - 1) return true;
		else if (y > screenHeight) return true;
		return false;
	}

	void update(double dt)
	{
		rx += dx;
		ry += dy;
		if (dx > 0)
		{
			while (rx > 0)
			{
				if (!willCollide(cx + 1, cy))
				{
					rx--;
					cx++;
				}
				else
				{
					dx = -dx * 0.1;
					rx = 0;
					break;
				}	
			}
		}
		else if (dx < 0)
		{
			while (rx <0)
			{
				if (!willCollide(cx -1, cy))
				{
					rx++;
					cx--;
				}
				else
				{
					dx = dx * 0.1;
					rx = 0;
					break;
				}
			}
		}
		if(playerState == PlayerState::Falling)
			ry += 0.5;
		if (dy > 0)
			while (ry > 0)
			{
				if (!willCollide(cx, cy))
				{
					ry++;
					cy--;
				}
				else
				{
					dy = -dy * 0.8;
					ry = 0.9;
					break;
				}
			}
		//dx *= 0.92;
		//dy *= 0.92;
		//if (pixely < 700)
			
		//else
			//playerState = PlayerState::Idle;
		Syncoord();
	}

	void	SetPosPixel(float pixelX, float pixelY)
	{
		cx = pixelX / CELL_WIDTH;
		cy = pixelY / CELL_HEIGHT;

		rx = (pixelX - (int)(cx * CELL_WIDTH)) / CELL_WIDTH;
		ry = (pixelY - (int)(cy * CELL_HEIGHT)) / CELL_WIDTH;
		Syncoord();
	}

	void	Syncoord()
	{
		pixelx = cx * CELL_WIDTH + rx * CELL_WIDTH;
		pixely = cy * CELL_HEIGHT + ry * CELL_HEIGHT;
		spr->setPosition(pixelx, pixely);
	}

	void draw(sf::RenderWindow & win) {
		sf::Text text;
		text.setCharacterSize(10);
		text.setPosition(pixelx + 20, pixely);
		text.setFont(*font);
		text.setString(std::to_string(pixelx) + ",  " + std::to_string(pixely));
		win.draw(text);
		win.draw(*spr);
	}
};
