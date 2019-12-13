#include "Game.hpp"
#include "Entity.hpp"
#include <SFML/Graphics.hpp>

Game* Game::me = nullptr;

static bool wasPressed[sf::Keyboard::KeyCount];

void Game::init()
{
	RectangleShape *sh = new RectangleShape(Vector2f(12, 24));
	sh->setFillColor(sf::Color::White);
	sh->setOrigin(6, 24);

	Entity * ent = new Entity(sh);
	ent->setPosPixel(100, 600);
	evec.push_back(ent);
	player = ent;
	me = this;

	memset(wasPressed, 0, sizeof(wasPressed));
	makePlatforms();
}


void Game::makePlatforms() {
	int cScreenWidth = 1280 / Entity::CELL_WIDTH;
	int cScreenHeight = 720 / Entity::CELL_WIDTH;

	for (int y = 0; y != cScreenHeight; y++)
	{
		for (int x = 0; x != cScreenWidth; x++)
		{
			if (y == 0 || y == cScreenHeight - 1 || x == 0 || x == cScreenWidth - 1)
				platforms.push_back(Vector2i(x, y));
		}
	}


	for (int y = 20; y != 21; y++)
	{
		for (int x = 30; x != 50; x++)
		{
			platforms.push_back(Vector2i(x, y));
		}
	}

	for (int y = 25; y != 26; y++)
	{
		for (int x = 10; x != 15; x++)
		{
			platforms.push_back(Vector2i(x, y));
		}
	}
}

void Game::update(double dt) {
	for (auto it = evec.begin(); it != evec.end();) {
		Entity * ent = *it;
		ent->update(dt);
		it++;
	}
	wasPressed[sf::Keyboard::Up] = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
}

void Game::draw(RenderWindow & win)
{
	for (auto it = evec.begin(); it != evec.end();) {
		Entity * ent = *it;
		ent->draw(win);
		it++;
	}

	auto CW = Entity::CELL_WIDTH;
	RectangleShape cell;
	cell.setFillColor(sf::Color(0x918EF0ff));
	cell.setSize(Vector2f(CW, CW));
	for (Vector2i & pl : platforms) {
		cell.setPosition(pl.x * CW, pl.y * CW);
		win.draw(cell);
	}

	for (auto it = pvec.begin(); it != pvec.end();) {
		Particle * p = *it;
		p->update();
		if (p->killed) {
			it = pvec.erase(it);
		}
		else {
			p->draw(win);
			it++;
		}
	}

	RectangleShape line(Vector2f(1000, 1));
	line.setFillColor(sf::Color::Green);
	line.setPosition(100, 600);
	win.draw(line);
}

bool Game::willCollide(Entity * end, int cx, int cy)
{
	int cScreenWidth = 1280 / Entity::CELL_WIDTH;
	int cScreenHeight = 720 / Entity::CELL_WIDTH;

	if (cx < 0) return true;
	else if (cx >= cScreenWidth) return true;

	if (cy < 0) return true;
	else if (cy >= cScreenHeight) return true;

	for (Vector2i & cell : platforms)
		if (cell.x == cx && cell.y == cy)
			return true;

	return false;
}

void Game::togglePlatform(int cx, int cy)
{
	for (auto it = platforms.begin(); it != platforms.end(); it++) {
		Vector2i & pl = *it;
		if (pl.x == cx && pl.y == cy) {
			platforms.erase(it);
			return;
		}
	}
	platforms.push_back(Vector2i(cx, cy));


}
