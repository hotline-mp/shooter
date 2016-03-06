/*
 * Entity.cpp
 * Copyright (C) 2016 jaume <jaume@altair>
 *
 * Distributed under terms of the MIT license.
 */

#include "Entity.hpp"

void Entity::setPosition(float x, float y) {
	this->position.x = x;
	this->position.y = y;
}

void Entity::setMoving(float x, float y) {
	this->moving.x = x;
	this->moving.y = y;
}

void Entity::setFacing(float x, float y) {
	this->facing.x = x;
	this->facing.y = y;
}

void Entity::draw() {
	// TODO
	const float player_r = 30.f;
    sf::CircleShape shape(player_r);
    shape.setFillColor(sf::Color::Green);
}

void Entity::update() {
	sf::Time time_now = clock.getElapsedTime();
	if (lastUpdated.asMicroseconds() == 0) {
		// El primer frame del joc ningú es mou
		lastUpdated = time_now;
		return;
	}
	long long micros = time_now.asMicroseconds() - lastUpdated.asMicroseconds();
	const float vel = 0.0003; // pixels / ms
	position.x += moving.x * vel * micros;
	position.y += moving.y * vel * micros;
	lastUpdated = clock.getElapsedTime();
}

