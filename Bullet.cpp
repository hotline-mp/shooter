#include <iostream>
#include <SFML/System/Vector2.hpp>
#include "Bullet.hpp"
#include "vector.hpp"

Bullet::Bullet(sf::Clock *clock, sf::RenderWindow *window, sf::Vector2f *camera) :
	Entity(clock, window, camera, 0.0006f /*vel*/, 5 /*radius*/),
	shape(radius)
{
	shape.setFillColor(sf::Color::Black);
	shape.setOrigin(radius, radius);
}

void Bullet::draw() {
	shape.setPosition(position + *camera);
	window->draw(shape);
}

bool circle_crossing_polygon_axis(sf::Vector2f point, float radius,
		std::vector<sf::Vector2f> polygon) {
	// iterem pels punts de dos en dos (segment a segment)
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

		float pointCp = dotProduct(point, axisUnit);
		float pointAp = pointCp + radius;
		float pointBp = pointCp - radius;
		float segmentAp = dotProduct(pointA, axisUnit);
		float segmentBp = dotProduct(pointB, axisUnit);
		// si ni un ni altre estan en mig, aquest costat falla i per tant no talla
		if ((between(pointAp, segmentAp, segmentBp) ||
					between(pointBp, segmentAp, segmentBp)) &&
				distanceLinePoint(pointA, pointB, point) < radius) {
			// Oh deu meu, estic fent un return dins un for sense fer un
			// recorregut complet! A l'infern directe, no?
			return true;
		}
	}
	return false;
}


void Bullet::update(std::vector< std::vector<sf::Vector2f> > map) {
	Entity::update();
	position += target_movement;
	if (!between(position.x, -2000, 2000) || !between(position.y, -2000, 2000)) {
		alive = false;
		return;
	}
	for (auto &polygon : map) {
		//if (point_in_polygon(position, polygon)) {
		if (circle_crossing_polygon_axis(position, radius, polygon)) {
			alive = false;
			return;
		}
	}
}
