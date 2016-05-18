#include <cmath>
#include "vector.hpp"

sf::Vector2f vecUnit(sf::Vector2f v) {
	float len = sqrt(pow(v.x, 2) + pow(v.y, 2));
	if (len == 0) {
		return sf::Vector2f(0, 0);
	}
	return v / len;
}

float dotProduct(sf::Vector2f u, sf::Vector2f v) {
	return u.x * v.x + u.y * v.y;
}

float crossProduct(sf::Vector2f u, sf::Vector2f v) {
	return u.x * v.y - u.y * v.x;
}

void projPoly(std::vector<sf::Vector2f> polygon,
		sf::Vector2f axis, float &min, float &max) {
	min = max = dotProduct(polygon[0], axis);
	for (int i=1; i<(int)polygon.size(); i++) {
		int p = dotProduct(polygon[i], axis);
		if (p < min) {
			min = p;
		} else if (p > max) {
			max = p;
		}
	}
}

void minMax(float &min, float &max) {
	float tmp;
	if (max < min) {
		tmp = min;
		min = max;
		max = tmp;
	}
}

bool between(float n, float a, float b) {
	minMax(a, b);
	return (n > a && n < b);
}

float distancePointPoint(sf::Vector2f A, sf::Vector2f B) {
	A -= B;
	return sqrt(pow(A.x, 2) + pow(A.y, 2));
}

float distanceLinePoint(sf::Vector2f A, sf::Vector2f B, sf::Vector2f P) {
	return fabs((B.y-A.y)*P.x - (B.x-A.x)*P.y + B.x*A.y - A.x*B.y) /
		sqrt(powf(B.y-A.y, 2) + powf(B.x-A.x, 2));
}

bool isPointInPoly(sf::Vector2f P, std::vector<sf::Vector2f> poly) {
	if (poly.size() < 2) {
		return false;
	}
	sf::Vector2f s1 = poly[1] - poly[0];
	bool side = (crossProduct(P - poly[0], s1) >= 0);
	for (int i=1; i<(int)poly.size(); i++) {
		sf::Vector2f p1 = poly[i];
		sf::Vector2f p2;
		if (i+1 == (int)poly.size()) {
			p2 = poly[0];
		} else {
			p2 = poly[i+1];
		}
		sf::Vector2f s = p2 - p1;
		if ((crossProduct((P - p1), s) >= 0) != side) {
			return false;
		}
	}
	return true;
}

bool lineCrossesLine(sf::Vector2f A, sf::Vector2f B,
		sf::Vector2f C, sf::Vector2f D) {
	return ((crossProduct(A - B, C - B) > 0) != (crossProduct(A - B, D - B) > 0) &&
			(crossProduct(C - D, A - D) > 0) != (crossProduct(C - D, B - D) > 0));
}

bool lineCrossesPoly(sf::Vector2f A, sf::Vector2f B,
		std::vector<sf::Vector2f> poly) {
	for (int i=0; i<(int)poly.size(); i++) {
		sf::Vector2f p1 = poly[i];
		sf::Vector2f p2;
		if (i+1 == (int)poly.size()) {
			p2 = poly[0];
		} else {
			p2 = poly[i+1];
		}
		if (lineCrossesLine(A, B, p1, p2)) {
			return true;
		}
	}
	return false;
}

