#include "Entity.hpp"
#include "vector.hpp"

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
	if (!visible) {
		return;
	}
    sf::CircleShape shape(radius);
	shape.setOrigin(sf::Vector2f(radius, radius));
    shape.setFillColor(sf::Color::Green);
}

void Entity::update() {
	sf::Time time_now = clock->getElapsedTime();
	if (lastUpdated.asMicroseconds() == 0) {
		// El primer frame del joc ningú es mou
		lastUpdated = time_now;
		return;
	}
	long long micros = time_now.asMicroseconds() - lastUpdated.asMicroseconds();
	target_movement = moving * float(vel * micros);
	lastUpdated = clock->getElapsedTime();
}

void Entity::pause() {
	lastUpdated = sf::Time(); // 0
}

void Entity::collisions(const std::vector< std::vector<sf::Vector2f> > &map) {
	if (target_movement == sf::Vector2f(0, 0)) {
		return;
	}
	sf::Vector2f target_position = this->position + this->target_movement;

	sf::Vector2f target_vector = this->target_movement;

	bool collision = false;
	//
	for (int ii=0; ii<2; ii++) {
		for (const std::vector<sf::Vector2f> &polygon : map) {
			if (polygon.size() < 2) {
				continue;
			}
			// iterem pels punts de dos en dos (segment a segment)
			std::vector< std::tuple<Point, Point> > lines_crossing;
			for (int i=0; i<(int)polygon.size(); i++) {
				sf::Vector2f pointA = polygon[i];
				sf::Vector2f pointB;
				if (i == (int)polygon.size()-1) {
					pointB = polygon[0];
				} else {
					pointB = polygon[i+1];
				}
				if (lineCrossesLine(position, target_position, pointA, pointB)) {
					if (collision) {
						// Fuck it, we ain't moving
						return;
					}
					lines_crossing.push_back(std::tuple<Point, Point>(pointA, pointB));
				}
			}
			if (lines_crossing.empty()) {
				continue;
			}
			bool passed_first = false;
			float mindist;
			Point closest;
			std::tuple<Point, Point> col_line;
			for (auto &line : lines_crossing) {
				Point p1;
				Point p2;
				std::tie(p1, p2) = line;
				Point c = lineLineIntersectionPoint(p1, p2, position, target_position);
				float dist = distancePointPoint(position, c);
				if (!passed_first || dist < mindist) {
					mindist = dist;
					closest = c;
					col_line = line;
					passed_first = true;
				}
			}
			Point cp1, cp2;
			std::tie(cp1, cp2) = col_line;
			sf::Vector2f newdir = vecUnit(cp2 - cp1);
			target_vector = newdir * dotProduct(target_vector, newdir);
			////newdir = newdir * dotProduct(newdir, closest - position);
			//target_vector = newdir * dotProduct(target_vector, newdir);
			target_position = this->position + target_vector;
			collision = true;
			break;

		}
		if (!collision) break;
	}

	this->position = target_position;
}

