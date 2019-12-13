#include "Entity.hpp"
#include "Game.hpp"
#include "Particle.h"
#include "FadingParticle.h"

using namespace std;
using namespace sf;

void Entity::setPosPixel(float pixelX, float pixelY) {
	cx = (int)pixelX / CELL_WIDTH;
	cy = (int)pixelY / CELL_WIDTH;

	rx = (pixelX - (cx*CELL_WIDTH)) / CELL_WIDTH;//reste de cx pour aller a rx
	ry = (pixelY - (cy*CELL_WIDTH)) / CELL_WIDTH;

	syncCoord();
}

void Entity::update(double dt) {
	float StateLife = 0;
	rx += dx;
	updateControl();
	///////X BLOCK 
	//maintain X cell coherency
	if (dx > 0)
		while (rx > 1) {
			if (!willCollide(cx + 1, cy)) {
				rx--;
				cx++;
			}
			else {
				dx = 0;
				rx = 0.9;
				break;
			}
		}
	else if (dx < 0)
		while (rx < 0) {
			if (!willCollide(cx - 1, cy)) {
				rx++;
				cx--;
			}
			else {
				dx = 0;
				rx = 0.1;
				break;
			}
		}

	//apply friction
	dx *= 0.92f;

	//cap dx min val to avoid sliding
	if (abs(dx) < 0.05) dx = 0; dy *= 0.92f;
	///////end bloc x

	ry += dy;
	//maintain Y cell coherency
	if (dy > 0)
		while (ry > 1) {
			if (!willCollide(cx, cy + 1)) {
				ry--;
				cy++;
			}
			else {
				dy = 0;
				ry = 0.99;
				break;
			}
		}
	else if (dy < 0)
		while (ry < 0) {
			if (!willCollide(cx, cy - 2)) {
				ry++;
				cy--;
			}
			else {
				dy = 0;
				ry = 0.01;
			}

		}
	if (abs(dy) < 0.05)
		dy = 0;
	dy *= 0.92f;
	auto max_vert_velocity = 2.0;
	if (dy >= max_vert_velocity)//cap vertical speed
		dy = max_vert_velocity;

	updateState();
	syncCoord();
}

void Entity::updateControl()
{
	auto lat_acc = 0.075;
	auto max_lat_speed = 0.75;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		dx += lat_acc;
		if (dx > max_lat_speed) dx = max_lat_speed;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		dx -= lat_acc;
		if (dx < -max_lat_speed) dx = -max_lat_speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		dy -= lat_acc;
		if (dy)
			dy = -max_lat_speed;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		dy += lat_acc;
		if (dy)
			dy = max_lat_speed;;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
		setPosPixel(100, 600);
		dx = dy = 0.0f;
	}

	switch (state)
	{
	case ES_IDLE:
		if (dx != 0 || dy != 0)
			changeState(ES_WALK);
		break;
	case ES_RUNNING:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		{
		}
		break;
	case ES_WALK:
		if ((dx == 0) && (dy == 0)) {
			changeState(ES_IDLE);
		}
		break;
	case ES_COVER:
	{
		bool isNearWall = willCollide(cx + 1, cy) || willCollide(cx - 1, cy) || willCollide(cx, cy + 1) || willCollide(cx, cy - 2);
		if (isNearWall == false)
			changeState(ES_IDLE);
	}
		break;
	default:
		break;
	}
}

void Entity::dropParticles() {
	int life = 30;
	float sp = Lib::rd() + 0.5;
	auto & g = *Game::me;

	float rdx = Lib::rd() * 6;
	float rdy = Lib::rd() * 6 - 8;
	{
		auto c = new CircleShape(8, 64);
		c->setOrigin(8, 8);
		c->setPosition(pixelX + rdx, pixelY + rdy);
		c->setFillColor(sf::Color(0xE580EB1a));
		auto p = new FadingParticle(c);
		p->life = 30;
		p->speed.y = sp;
		p->bhv = Particle::applySpeed;
		g.pvec.push_back(p);

	}

	{
		auto c = new CircleShape(2, 32);
		c->setOrigin(2, 2);
		c->setPosition(pixelX + rdx, pixelY + rdy);
		c->setFillColor(sf::Color(0xE580EBff));

		auto p = new FadingParticle(c);
		p->life = 30;
		p->speed.y = sp;
		p->bhv = Particle::applySpeed;
		g.pvec.push_back(p);
	}
}

void Entity::draw(sf::RenderWindow & win) {

	sf::Text coords;
	coords.setFont(*Game::me->fnt);
	coords.setPosition(pixelX + 20, pixelY - 20);
	coords.setFillColor(sf::Color::Red);
	coords.setString(
		std::to_string(cx) + "," + std::to_string(cy)
		+ " " + std::to_string(rx) + "," + std::to_string(ry)
		+ " " + getStateName());

	win.draw(*spr);
	win.draw(coords);
}

void Entity::updateState()
{
	switch (state)
	{
	case ES_IDLE:
	{
		spr->setFillColor(Color::White);
		bool isNearWall = willCollide(cx + 1, cy) || willCollide(cx - 1, cy) || willCollide(cx, cy + 1) || willCollide(cx, cy - 2);
		if (stateLife >= 60 && isNearWall)
			changeState(ES_COVER);
	}
		break;
	case ES_RUNNING:
		spr->setFillColor(Color::Blue);
		dropParticles();
		break;
	case ES_WALK:
		spr->setFillColor(Color::Blue);
		break;
	case ES_COVER:
		spr->setFillColor(Color::Green);
		break;
	default:
		break;
	}
	stateLife++;
	
}

void Entity::changeState(EntityState nes)
{
	if (state == nes)
		return;
	stateLife = 0;
	state = nes;
}

bool Entity::willCollide(int cx, int cy)
{
	return Game::me->willCollide(this, cx, cy);
}

std::string Entity::getStateName() {
	switch (state)
	{
	case ES_IDLE:
		return "idle";
		break;
	case ES_RUNNING:
		return "run";
		break;
	case ES_WALK:
		return "walk";
		break;
	case ES_COVER:
		return "cover";
		break;
	}
}
