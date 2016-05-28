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

void Particle::update(std::vector< std::vector<sf::Vector2f> > map) {
	//Entity::update();

	sf::Time time_now = clock->getElapsedTime();
	if (lastUpdated.asMicroseconds() == 0) {
		// El primer frame del joc ningú es mou
		lastUpdated = time_now;
		return;
	}
	long long micros = time_now.asMicroseconds() - lastUpdated.asMicroseconds();
	sf::Vector2f a = vecUnit(vvel) * float(accel * micros);
	sf::Vector2f r = vvel + a;
	if ((vvel.x > 0 && r.x < 0) || (vvel.x < 0 && r.x > 0) ||
			(vvel.y > 0 && r.y < 0) || (vvel.y < 0 && r.y > 0)) {
		vvel = sf::Vector2f(0, 0);
		accel = 0;
		alive = false;
	} else {
		vvel = r;
	}
	target_movement = vvel * float(micros);
	lastUpdated = clock->getElapsedTime();

	position += target_movement;
	if (!between(position.x, -2000, 2000) || !between(position.y, -2000, 2000)) {
		alive = false;
		return;
	}
	for (auto &polygon : map) {
		//if (point_in_polygon(position, polygon)) {
		if (circleCrossingPolygonAxis(position, std::max(radius, 3.f), polygon) ||
				isPointInPoly(position, polygon)) {
			alive = false;
			return;
		}
	}
}
