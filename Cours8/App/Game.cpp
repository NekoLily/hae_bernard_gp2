#include "Game.hpp"

void Game::init()
{
	RectangleShape* rs = new RectangleShape(Vector2f(12, 24));
	rs->setFillColor(Color::Magenta);
	rs->setOrigin(6, 24);
	Entity * ent = new Entity(rs);
	ent->SetPosPixel(100, 700);
	evec.push_back(ent);
	Player = ent;
}



void Game::update(double dt)
{
	float vel = 0.5;
	float max_vel = 0.5f;
	for (auto it = evec.begin(); it != evec.end();) 
	{
		Entity * e = *it;
		e->update(dt);
		it++;
	}

	if (Keyboard::isKeyPressed(Keyboard::Right))
	{
		Player->dx += vel;
		if (Player->dx > max_vel)
			Player->dx = max_vel;
	}
	if(Keyboard::isKeyPressed(Keyboard::Left))
	{
		Player->dx -= vel;
		if (Player->dx < max_vel)
			Player->dx = -max_vel;
	}
	if (Keyboard::isKeyPressed(Keyboard::Up) && Player->playerState != PlayerState::Falling)
	{
		Player->dy -= 0.7;
		Player->playerState = PlayerState::Falling;
	}
}

void Game::draw(RenderWindow & win)
{
	for (auto it = evec.begin(); it != evec.end();)
	{
		Entity * e = *it;
		e->font = this->font;
		e->draw(win);
		it++;
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
	line.setPosition(100, 700);
	line.setFillColor(Color::Green);
	win.draw(line);
}
