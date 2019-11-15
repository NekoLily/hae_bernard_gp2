#include <string>
#include <SFML/Graphics.hpp>
#include <iostream> 
#include <list>
#include <iterator> 
#include "pch.h"
#include "Lib.h"
#include "Char.h"

using namespace sf;
using namespace std;

static std::vector<Char> CharList;
int offsetSpeed = 3;
sf::Color hsv(int hue, float sat, float val)
{
	hue %= 360;
	while (hue < 0) hue += 360;

	if (sat < 0.f) sat = 0.f;
	if (sat > 1.f) sat = 1.f;

	if (val < 0.f) val = 0.f;
	if (val > 1.f) val = 1.f;

	int h = hue / 60;
	float f = float(hue) / 60 - h;
	float p = val * (1.f - sat);
	float q = val * (1.f - sat * f);
	float t = val * (1.f - sat * (1 - f));

	switch (h)
	{
	default:
	case 0:
	case 6: return sf::Color(val * 255, t * 255, p * 255);
	case 1: return sf::Color(q * 255, val * 255, p * 255);
	case 2: return sf::Color(p * 255, val * 255, t * 255);
	case 3: return sf::Color(p * 255, q * 255, val * 255);
	case 4: return sf::Color(t * 255, p * 255, val * 255);
	case 5: return sf::Color(val * 255, p * 255, q * 255);
	}
}

/*void DrawSquare(sf::RenderWindow &win, float now)
{
	sf::RectangleShape rect(Vector2f(30, 30));
	rect.setPosition(PlayerPos);
	win.draw(rect);
}*/

void drawCatmull(sf::RenderWindow &win, float now) {
	sf::VertexArray va(sf::LineStrip);
	sf::Color red = sf::Color::Red;
	sf::Color blue = sf::Color::Blue;
	int nb = 320;
	float stride = 1280.0 / nb;

	std::vector<Vector2f> points;

	for (int j = 0; j < 8; ++j) {
		Vector2f v(j * 100, j * 100);
		if (j == 0)v.x += 100;
		if (j == 3)v.x += 200;
		points.push_back(v);
	}

	sf::CircleShape shape(10.f);
	shape.setFillColor(sf::Color(0x007fffff));
	shape.setOutlineColor(sf::Color(0x7fff73fff));
	static bool forward = true;
	static float current = 0;
	if (forward)
		current += 0.01f;
	else current -= 0.01f;
	if (current >= 1)forward = false;
	else if (current <= 0) forward = true;

	for (int i = 0; i < nb + 1; ++i) {
		double ratio = 1.0 * i / nb;
		double x = 0.0;
		double y = 0.0;
		sf::Color c = hsv(ratio * 360, 0.8, 0.8);
		Vector2f pos = Lib::plot2(ratio, points);
		x = pos.x;
		y = pos.y;
		sf::Vertex vertex(Vector2f(x, y), c);
		va.append(vertex);
	}
	shape.setPosition(Lib::plot2(current, points));
	win.draw(shape);
	win.draw(va);
}

void DrawCurve(sf::RenderWindow &win, float now)
{
	VertexArray va(sf::LineStrip);
	Color red = Color::Red;
	Color blue = Color::Blue;
	int nb = 500;
	float stride = 600.0f / nb;
	int offX = 0;
	int offY = 400;
	double y = offY;
	for (int i = 0; i < nb; i++)
	{
		double ratio = 1.0 * i / nb;
		double x = offX + stride + i;
		y += sin((ratio * 10 + now * 3.14) * 2);
		Vertex vertex(Vector2f(x, y), i % 2 == 0 ? red : blue);
		va.append(vertex);
	}
	win.draw(va);
}

void DrawCircle(sf::RenderWindow &win)
{
	sf::CircleShape shape(100.f);
	shape.setPosition(50, 50);
	shape.setFillColor(sf::Color(0x007fffff));
	shape.setOutlineColor(sf::Color(0x7fff73fff));
	win.draw(shape);
}

void DrawChar(sf::RenderWindow &win)
{
	for (Char &elemen : CharList)
	{
		win.draw(elemen.tank);
		elemen.SetPosition();
	}
}

/*bool Shoot(sf::RenderWindow &win, float now)
{
	sf::VertexArray va(sf::LineStrip);
	sf::Color red = sf::Color::Red;
	sf::Color blue = sf::Color::Blue;
	std::vector<Vector2f> points;
	points.push_back(PlayerPos);
	points.push_back(Vector2f(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y));
	sf::CircleShape shape(10.f);
	shape.setFillColor(sf::Color(0x007fffff));
	shape.setOutlineColor(sf::Color(0x7fff73fff));
	static float current = 0;
	if (current <= 1)current += 0.01f;
	else
		return false;
	shape.setPosition(Lib::plot2(current, points));
	win.draw(shape);
	return true;
}*/
Vector2f Beta;
void World(sf::RenderWindow &win)
{
	/*sf::Vector2f Lastposition;
	for (Char &elemen : CharList)
	{
		CharList[0].GetBoundingBox();
		elemen.GetBoundingBox();
		if (elemen.name != "Player" && CharList[0].boundingBox.intersects(elemen.boundingBox))
		{
			Hit = true;
			offsetSpeed = 0.0f;
			
			printf("Hit %s  Position x = %f Y = %f\   LastPosition  x = %f y = %f \n", elemen.name, elemen.position.x, elemen.position.y, CharList[0].lastPosition.x, CharList[0].lastPosition.y);
			CharList[0].position = Lastposition;
			CharList[0].SetPosition();
			Hit = false;
			//offsetSpeed = 5.0f;
		}
		else if (elemen.name == "Player")
		{
			if (Hit == false)
			{
				CharList[0].SetPosition();
				Lastposition = CharList[0].position;
			}
		}
	}*/

	if (CharList[0].tank.getGlobalBounds().intersects(CharList[1].tank.getGlobalBounds()))
	{
		CharList[0].position.x = Beta.x - 3;
		CharList[0].position.y = Beta.y;
		offsetSpeed = 0;
	}
	else
	{
		Beta.x = CharList[0].position.x;
		Beta.y = CharList[0].position.y;
		offsetSpeed = 3;
	}
	DrawChar(win);
}

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 2;
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
	sf::Clock clock;
	sf::Time time = clock.getElapsedTime();
	sf::Time FrameStart;
	sf::Time FrameEnd;
	sf::Time LastFrame;

	sf::Text FPStext;
	sf::Text MousePosText;
	sf::Text PlayerPosText;

	sf::Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");

	FPStext.setFont(font);
	FPStext.setFillColor(sf::Color::Red);

	PlayerPosText.setPosition(500, 0);
	PlayerPosText.setFont(font);
	PlayerPosText.setFillColor(sf::Color::Red);

	MousePosText.setPosition(1000, 0);
	MousePosText.setFont(font);
	MousePosText.setFillColor(sf::Color::Red);


	float FPS;
	window.setVerticalSyncEnabled(true);
	

	Char Player = Char(Vector2f(50, 50), Vector2f(30, 30), "Player", Color::Blue);
	Char Bot = Char(Vector2f(80, 80), Vector2f(30, 30), "Bot 1", Color::Red);
	CharList.push_back(Player);
	CharList.push_back(Bot);
	while (window.isOpen())
	{
		FrameStart = clock.getElapsedTime();
		FPS = 1.0f / (FrameStart - LastFrame).asSeconds();
		sf::Event event;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))CharList[0].position.x += offsetSpeed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))CharList[0].position.x -= offsetSpeed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))CharList[0].position.y -= offsetSpeed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))CharList[0].position.y += offsetSpeed;
		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))Shoot(window, FrameStart.asSeconds());
		while (window.pollEvent(event))
		{
			switch (event.key.code)
			{
			case sf::Keyboard::E:
				printf("FPS : %f\n", FPS);
				break;
			default:
				break;
			}
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear();

		FPStext.setString(std::to_string(FPS));
		MousePosText.setString(to_string(sf::Mouse::getPosition().x) + " " + to_string(sf::Mouse::getPosition().y));
		PlayerPosText.setString(to_string(CharList[0].position.x) + " " + to_string(CharList[0].position.y));

		window.draw(PlayerPosText);
		window.draw(MousePosText);
		window.draw(FPStext);

		World(window);

		window.display();
		FrameEnd = clock.getElapsedTime();
		LastFrame = FrameStart;
	}
	return 0;
}