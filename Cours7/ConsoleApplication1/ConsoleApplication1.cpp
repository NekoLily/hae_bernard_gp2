#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <List>
#include <cstdio>
#include "pch.h"
#include "Lib.h"

enum Action
{
	Forward, Left, Right, Backward 
};

using namespace sf;

class Turtle : public sf::ConvexShape {
public:
	sf::Transform m_Trs;

	Turtle() : sf::ConvexShape(3) {
		setFillColor(sf::Color(0x75FF30ff));
		setOutlineThickness(2);
		setOutlineColor(sf::Color(0xcd, 0xcd, 0xcd));

		setPoint(0, Vector2f(0, -24));
		setPoint(1, Vector2f(-16, 16));
		setPoint(2, Vector2f(16, 16));
	}

	void setTransform(sf::Transform trs) {
		m_Trs = trs;
	}

};

static sf::Shader * simpleShader = nullptr;

static sf::Texture * whiteTex = nullptr;
static sf::Texture * gradientMountain = nullptr;

static sf::Transform				s_Initial;
static std::vector<sf::Transform>	s_Trs;
static std::vector<Turtle*>			s_Turtles;

static char data[1024];

static std::vector<Action> commandList;

static float LastTime = 0;
static time_t LastTimeModification;

bool startsWith(const char * s0, const char * s1) {
	if (*s0 == 0 && *s1 != 0)
		return false;
	if (*s1 == 0)
		return true;
	if (*s0 != *s1)
		return false;
	else
		return startsWith(s0 + 1, s1 + 1);
}

static void startTransforms() {
	s_Initial = Transform::Identity;
	s_Initial.translate(500, 500);
}

static void translateX(float dx) {
	sf::Transform res;
	res.translate(dx, 0);
	s_Trs.push_back(res);
}

static void translateY(float dy) {
	sf::Transform res;
	res.translate(0, dy);
	s_Trs.push_back(res);
}

static void rotate(float degrees) {
	sf::Transform res;
	res.rotate(degrees);
	s_Trs.push_back(res);
}

static void scaleXY(float dx, float dy) {
	sf::Transform res;
	res.scale(dx, dy);
	s_Trs.push_back(res);
}

static void scaleXY(float dxy) {
	sf::Transform res;
	res.scale(dxy, dxy);
	s_Trs.push_back(res);
}

static void scaleX(float dx) {
	sf::Transform res;
	res.scale(dx, 0);
	s_Trs.push_back(res);
}

static void scaleY(float dy) {
	sf::Transform res;
	res.scale(0, dy);
	s_Trs.push_back(res);
}

static void computeTransform(sf::Transform & result, int step = -1) {
	sf::Transform inter;

	inter.combine(s_Initial);

	if (step <= -1) {
		for (sf::Transform t : s_Trs)
			inter = inter.combine(t);
	}
	else {
		step--;
		for (sf::Transform t : s_Trs) {
			inter = inter.combine(t);
			if (step <= 0)
				break;
		}
	}
	result = inter;
}

static void plotTurtle() {
	sf::Transform cur;
	computeTransform(cur, s_Trs.size() - 1);
	Turtle* t = new Turtle();
	t->setTransform(cur);
	s_Turtles.push_back(t);
}

static	void ReadScript()
{
	FILE *f = nullptr;
	auto err = fopen_s(&f, "res/scriptTortue.txt", "r");
	if (f != nullptr)
	{
		memset(data, 0, 1024);
		fread(data, 2014, 1, f);
		printf("Read file %s\n", data);
		fclose(f);
		char *cur = data;
		bool doContinue = true;
		while (doContinue)
		{
			if (*cur != 0)
			{
				if (startsWith(cur, "FW"))
					commandList.push_back(Action::Forward);
				else if (startsWith(cur, "BW"))
					commandList.push_back(Action::Backward);
				else if (startsWith(cur, "L"))
					commandList.push_back(Action::Left);
				else if (startsWith(cur, "R"))
					commandList.push_back(Action::Right);
			}
			else
				doContinue = false;
			cur++;
		}
	}
	else
		printf("Cannot read files %s %d\n", "res/scriptTortue.txt", err);
}

static void Repeater(float time)
{
	if (LastTime = 0)
	{
		LastTime = time;
		ReadScript();
	}
	else if (time - LastTime > 1)
	{
		struct stat sb;
		stat("res/scriptTortue.txt", &sb);
		printf("%f", &sb.st_mtime);
		ReadScript();
	}
	
}

int main() {

	sf::ContextSettings settings;
	settings.antialiasingLevel = 2;

	sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);

	sf::View initialView = window.getDefaultView();
	sf::View mainView = window.getDefaultView();
	window.setView(mainView);
	//ImGui::SFML::Init(window);

	sf::Clock clock;

	sf::Time appStart = clock.getElapsedTime();
	sf::Time frameStart = clock.getElapsedTime();
	sf::Time prevFrameStart = clock.getElapsedTime();

	float fps[4] = { 0.f,0.f,0.f,0.f };
	int step = 0;
	sf::Font * font = new sf::Font();
	if (font->loadFromFile("res/DejaVuSans.ttf") == false) {
		printf("no such font\n");
	}

	if (!sf::Shader::isAvailable())
		printf("no shader avail\n");

	simpleShader = new Shader();
	if (!simpleShader->loadFromFile("res/simple.vert", "res/simple.frag"))
		printf("unable to load shaders\n");

	whiteTex = new Texture();
	if (!whiteTex->create(1, 1)) printf("tex crea failed\n");
	whiteTex->setSmooth(true);
	unsigned int col = 0xffffffff;
	whiteTex->update((const sf::Uint8*)&col, 1, 1, 0, 0);

	gradientMountain = new Texture();
	if (!gradientMountain->loadFromFile("res/mountain.png")) {
		printf("unable to load tex\n");
	}

	sf::Text myFpsCounter;
	int every = 0;

	sf::Clock deltaClock;

	double winWidth = window.getSize().x;
	double winHeight = window.getSize().y;

	int showSegment = 0;

	startTransforms();
	plotTurtle();

	ReadScript();
	
	for (Action commande : commandList)
	{
		switch (commande)
		{
		case Forward:
			translateY(-50);
			break;
		case Left:
			rotate(-45);
			break;
		case Right:
			rotate(45);
			break;
		case Backward:
			translateY(50);
			break;
		default:
			break;
		}
		plotTurtle();
	}

	while (window.isOpen())//on passe tout le temps DEBUT DE LA FRAME 
	{
		
		sf::Event event;//recup les evenement clavier/pad
		frameStart = clock.getElapsedTime();
		window.setView(initialView);

		window.clear();
		while (window.pollEvent(event)) {

			//ImGui::SFML::ProcessEvent(event);

			Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));

			switch (event.type) {

			case sf::Event::Resized:
				initialView.setSize({
								   static_cast<float>(event.size.width),
								   static_cast<float>(event.size.height)
					});
				window.setView(initialView);
				break;

			case sf::Event::KeyReleased: {

				if (event.key.code == sf::Keyboard::Space) {
					startTransforms();

					sf::Transform trs;
					computeTransform(trs);

					for (Turtle * t : s_Turtles) delete t;
					s_Turtles.clear();

					Turtle* t = new Turtle();
					t->setTransform(trs);
					s_Turtles.push_back(t);
				}

				auto delta = 32;

				if (event.key.code == sf::Keyboard::Up) { translateY(-delta);	   plotTurtle(); }
				if (event.key.code == sf::Keyboard::Down) { translateY(delta);	   plotTurtle(); }

				if (event.key.code == sf::Keyboard::Left) { rotate(-45);			   plotTurtle(); }
				if (event.key.code == sf::Keyboard::Right) { rotate(45);			   plotTurtle(); }

				if (event.key.code == sf::Keyboard::Add) { scaleXY(0.5);			   plotTurtle(); }
				if (event.key.code == sf::Keyboard::Subtract) { scaleXY(2.0);			   plotTurtle(); }

				break;
			}

			case sf::Event::KeyPressed:

				break;

			case sf::Event::Closed:
				window.close();
				break;

			default:
				break;
			}
		}

		sf::Time dt = deltaClock.restart();
		//ImGui::SFML::Update(window, dt);

		myFpsCounter.setPosition(8, 8);
		myFpsCounter.setFillColor(sf::Color::Red);
		myFpsCounter.setFont(*font);

		if (every == 0) {
			myFpsCounter.setString(std::string("FPS:") + std::to_string(fps[(step - 1) % 4]));
			every = 30;
		}
		every--;

		window.draw(myFpsCounter);

		for (Turtle * t : s_Turtles) {
			RenderStates rs;
			rs.transform = t->m_Trs;
			window.draw(*t, rs);
		}

		//ImGui::SFML::Render(window);
		window.display();//ca dessine et ca attend la vsync

		fps[step % 4] = 1.0f / (frameStart - prevFrameStart).asSeconds();
		prevFrameStart = frameStart;

		step++;
	}

	//ImGui::SFML::Shutdown();

	return 0;
}