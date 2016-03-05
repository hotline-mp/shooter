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

void Entity::draw(sf::RenderWindow &windows) {
	const float player_r = 30.f;
    sf::CircleShape shape(player_r);
    shape.setFillColor(sf::Color::Green);
}

