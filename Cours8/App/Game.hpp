#pragma once


#include <iostream>
#include <SFML/Graphics.hpp>
#include <direct.h>
#include <functional>

#include "Lib.hpp"
#include "Particle.h"
#include "Action.hpp"
#include <Box2D/Box2D.h>
#include "Entity.hpp"

using namespace sf;



class Game {
public:
	Entity * Player;
	sf::Font *font;
	std::vector< Particle * > pvec;
	std::vector<Entity*> evec;

	void init();
	void update(double t);
	bool willCollide(Entity * ent, int x, int y);
	void draw(sf::RenderWindow & win);
};
