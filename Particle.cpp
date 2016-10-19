#include <iostream>
#include <SFML/System/Vector2.hpp>
#include "Particle.hpp"
#include "vector.hpp"
#include <algorithm>

Particle::Particle(sf::Clock *clock, sf::RenderWindow *window, float radius) :
	Particle(clock, window, radius, sf::Vector2f(0, 0), 0)
{
}

Particle::Particle(sf::Clock *clock, sf::RenderWindow *window, float radius,
		sf::Vector2f vel, float accel, sf::Color color) :
	Entity(clock, window),
	vvel(vel),
	shape(radius),
	accel(accel)
{
	this->radius = radius;
	shape.setFillColor(color);
	shape.setOrigin(radius, radius);
}

void Particle::draw() {
	shape.setPosition(position);
	window->draw(shape);
}

void Particle::update(const std::vector< std::vector<sf::Vector2f> > &map) {
	// TODO: simplify, make faster
	if (vvel == sf::Vector2f(0, 0) || vecLength(vvel) < .00001f) {
		return;
	}

	sf::Time time_now = clock->getElapsedTime();
	if (lastUpdated.asMicroseconds() == 0) {
		// El primer frame del joc ningú es mou
		lastUpdated = time_now;
		return;
	}
	long long micros = time_now.asMicroseconds() - lastUpdated.asMicroseconds();
	sf::Vector2f a = vecUnit(vvel) * float(accel * micros);
	if (vecLength(a) < float(accel * micros)) {
		vvel = sf::Vector2f(0, 0);
		accel = 0;
		if (disappear) {
			alive = false;
		}
		return;
	} else {
		vvel += a;
	}

	target_movement = vvel * float(micros);

	lastUpdated = clock->getElapsedTime();

	prev_pos = position;
	position += target_movement;
	if (!between(position.x, -20000, 20000) || !between(position.y, -20000, 20000)) {
		alive = false;
		return;
	}
	if (collide) {
		for (auto &polygon : map) {
			// TODO: Try to call this less
			if (lineCrossesPoly(prev_pos, position, polygon)) {
				vvel = sf::Vector2f(0, 0);
				if (disappear) {
					alive = false;
				}
				return;
			}
		}
	}
}
