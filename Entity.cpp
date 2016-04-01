/*
 * Entity.cpp
 * Copyright (C) 2016 jaume <jaume@altair>
 *
 * Distributed under terms of the MIT license.
 */

#include "Entity.hpp"
#include "vector.hpp"

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
	target_movement = moving * float(vel * micros);
	lastUpdated = clock.getElapsedTime();
}

void Entity::collisions(std::vector< std::vector<sf::Vector2f> > map) {
	/* colisions TODO: arreglar */
	sf::Vector2f player_center = this->position + this->target_movement +
		sf::Vector2f(player_r, player_r);

	int pol_i = 0;
	bool collision = false;
	sf::Vector2f ctarget = this->target_movement;

	for (std::vector<sf::Vector2f> polygon : map) {
		int seg_i = 0;
		for (int i=0; i<(int)polygon.size(); i++) {
			sf::Vector2f pointA = polygon[i];
			sf::Vector2f pointB;
			if (i == (int)polygon.size()-1) {
				pointB = polygon[0];
			} else {
				pointB = polygon[i+1];
			}
			sf::Vector2f axis = pointB - pointA;
			sf::Vector2f axisUnit = vecUnit(axis);
			float segmentAp = dotProduct(pointA, axis);
			float segmentBp = dotProduct(pointB, axis);
			if (distancePointPoint(pointA, player_center) < player_r * 1.1) {
				//std::cout << "..." << std::endl;
				sf::Vector2f c = player_center - pointA;
				sf::Vector2f tan = sf::Vector2f(c.y, -c.x);
				//std::cout << tan.x << " " << tan.y << std::endl;
				sf::Vector2f tanu = vecUnit(tan);
				sf::Vector2f tanp = tanu * dotProduct(vecUnit(ctarget), tanu);
				if (tanp == sf::Vector2f(0, 0) ||
						dotProduct(-vecUnit(c), vecUnit(ctarget)) > 0.95f) {
					ctarget = sf::Vector2f(0, 0);
					//std::cout << "lmao0" << std::endl;
				} else {
					//std::cout << tanp.x << " " << tanp.y << std::endl;
					sf::Vector2f tan_oberta = vecUnit(vecUnit(tanp) + vecUnit(c)*0.25f);
					//std::cout << tan_oberta.x << " " << tan_oberta.y << std::endl;

					ctarget = tan_oberta * dotProduct(ctarget, tan_oberta);
					player_center = this->position + ctarget +
						sf::Vector2f(player_r, player_r);
					//std::cout << "lmao1" << std::endl;
				}
			}
			float playerP = dotProduct(player_center, axis);
			if ((between(playerP, segmentAp, segmentBp) &&
					distanceLinePoint(pointA, pointB, player_center) < player_r) ||
					distancePointPoint(pointA, player_center) < player_r ||
					distancePointPoint(pointB, player_center) < player_r) {
					//std::cout << "lmao2" << std::endl;
				//std::cout << "collision " << i << std::endl;
				//std::cout << axisUnit.x << " " << axisUnit.y << std::endl;
				collision = true;
				ctarget = axisUnit * dotProduct(ctarget, axisUnit);
				player_center = this->position + ctarget +
					sf::Vector2f(player_r, player_r);
			}
		}
		pol_i++;
	}

	this->position = player_center - sf::Vector2f(player_r, player_r);

}

