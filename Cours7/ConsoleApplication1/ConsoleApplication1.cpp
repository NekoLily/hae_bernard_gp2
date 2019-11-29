


#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "pch.h"
#include "Lib.h"

using namespace sf;

static sf::Vector2i F1Pos(0, 0);
static sf::Vector2i F2Pos(0, 0);
static sf::Vector2i F3Pos(0, 0);
static sf::Vector2i F4Pos(0, 0);
static Vector2f RectPos = Vector2f(800, 450);
//static std::vector<Vector2f> points;

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

void DrawSquare(sf::RenderWindow &win, float now)
{
	
}

void drawCatmull(sf::RenderWindow &win, float now) {
	sf::VertexArray va(sf::LineStrip);
	sf::Color red = sf::Color::Red;
	sf::Color blue = sf::Color::Blue;
	int nb = 320;
	float stride = 1280.0 / nb;

	std::vector<Vector2f> points;

	/*for (int j = 0; j < 8; ++j) {
		Vector2f v(j * 100, j * 100);
		if (j == 0)v.x += 100;
		if (j == 3)v.x += 200;
		points.push_back(v);
	}*/

	for (int j = 0; j < 8; j++) {
		Vector2f v(0, 0);
		if (j == 1)v.x += F1Pos.x;
		if (j == 1)v.y += F1Pos.y;
		if (j == 2)v.x += F2Pos.x;
		if (j == 2)v.y += F2Pos.y;
		if (j == 3)v.x += F3Pos.x;
		if (j == 3)v.y += F3Pos.y;
		if (j == 4)v.x += F4Pos.x;
		if (j == 4)v.y + F4Pos.y;
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

void Force(sf::RenderWindow &win, float now)
{
	VertexArray va(sf::LineStrip);
	Color red = Color::Red;
	Color blue = Color::Blue;
	int nb = 500;
	float stride = 600.0f / nb;
	int offX = RectPos.x + 15;
	int offY = RectPos.y + 15;
	double y = offY;
	for (int i = 0; i < nb; i++)
	{
		double ratio = 1.0 * i / nb;
		double x = offX + stride + i;
		x += cos((ratio * 10 + now * 3.14) * 2);
		y += sin((ratio * 10 + now * 3.14) * 2);
		Vertex vertex(Vector2f(x, y), i % 2 == 0 ? red : blue);
		va.append(vertex);
	}
	win.draw(va);
}

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 2;
	sf::RenderWindow window(sf::VideoMode(1600, 900), "SFML works!");


	sf::Shader shader;
	if (!sf::Shader::isAvailable())
	{
		printf("Shader non disponible");
	}
	
	if (!shader.loadFromFile("simple.vert", "simple.frag"))
	{
		printf("Impossible de charger les shaders");
	}
	
	static	sf::Texture *whiteTex =  nullptr;
	whiteTex = new Texture();
	if (!whiteTex->create(1, 1))printf("tex crea failed \n");
	whiteTex->setSmooth(true);
	unsigned int col = 0xffffffff;
	whiteTex->update((const sf::Uint8*)&col, 1, 1, 0, 0);
	
	
	sf::Clock clock;
	sf::Time time = clock.getElapsedTime();
	sf::Time FrameStart;
	sf::Time FrameEnd;
	sf::Time LastFrame;
	sf::Text text;
	sf::Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
	text.setFont(font);
	text.setFillColor(sf::Color::Red);
	float FPS;
	window.setVerticalSyncEnabled(true);
	float offsetSpeed = 5.0f;
	sf::RectangleShape rect(Vector2f(50, 50));
	
	
	rect.setTexture(whiteTex);

	while (window.isOpen())
	{
		window.clear();
		FrameStart = clock.getElapsedTime();
		FPS = 1.0f / (FrameStart - LastFrame).asSeconds();
		sf::Event event;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))RectPos.x += offsetSpeed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))RectPos.x -= offsetSpeed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))RectPos.y -= offsetSpeed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))RectPos.y += offsetSpeed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))Force(window, FrameStart.asSeconds());
		while (window.pollEvent(event))
		{
			//if (event.type == sf::Event::KeyReleased)
			switch (event.key.code)
			{
			case sf::Keyboard::E:
				printf("FPS : %f\n", FPS);
				break;
			case sf::Keyboard::F1:
				F1Pos = sf::Mouse::getPosition();;
				break;
			case sf::Keyboard::F2:
				F2Pos = sf::Mouse::getPosition();;
				break;
			case sf::Keyboard::F3:
				F3Pos = sf::Mouse::getPosition();;
				break;
			case sf::Keyboard::F4:
				F4Pos = sf::Mouse::getPosition();;
				break;
			default:
				break;
			}
			if (event.type == sf::Event::Closed)
				window.close();
		}
		text.setString(std::to_string(FPS));

		rect.setPosition(RectPos);
		shader.setUniform("time", FrameStart.asSeconds());
		window.draw(rect, &shader);

		window.draw(text);
		window.display();
		FrameEnd = clock.getElapsedTime();
		LastFrame = FrameStart;
	}
	return 0;
}