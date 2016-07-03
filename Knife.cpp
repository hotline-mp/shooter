#include "Knife.hpp"
#include "vector.hpp"
#include <math.h>
#include <iostream>

Knife::Knife(sf::Texture *texture, sf::Clock *clock, sf::RenderWindow *window, sf::Vector2f vel) :
	Entity(clock, window),
	vvel(vel)
{
	sprite.setTexture(*texture);
    sprite.setOrigin(16, 16);
	radius = 12;
	//vvel = vel;
    const float PI = 3.14159265;
    float rotation = (atan2(vel.y, vel.x)) * 180 / PI + 90;
	sprite.setRotation(rotation);
}

void Knife::draw() {
	if (!visible) {
		return;
	}
    sprite.setPosition(position);

	window->draw(sprite);
}


void Knife::update(std::vector< std::vector<sf::Vector2f> > map) {
	sf::Time time_now = clock->getElapsedTime();
	if (lastUpdated.asMicroseconds() == 0) {
		// El primer frame del joc ningú es mou
		lastUpdated = time_now;
		return;
	}
	long long micros = time_now.asMicroseconds() - lastUpdated.asMicroseconds();
	target_movement = vvel * float(micros);
	lastUpdated = clock->getElapsedTime();

	sf::Vector2f prev_pos = position;
	position += target_movement;
	if (!between(position.x, -20000, 20000) || !between(position.y, -20000, 20000)) {
		alive = false;
		return;
	}
	for (auto &polygon : map) {
		//if (point_in_polygon(position, polygon)) {
		if (lineCrossesPoly(prev_pos, position, polygon)) {
		//if (circleCrossingPolygonAxis(position, std::max(radius, 3.f), polygon) ||
		//		isPointInPoly(position, polygon)) {
			//alive = false;
			vvel = sf::Vector2f(0, 0);
			position = prev_pos;
			return;
		}
	}
}
