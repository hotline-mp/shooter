#include <cmath>
#include "vector.hpp"

float vecLength(sf::Vector2f v) {
	return sqrt(pow(v.x, 2) + pow(v.y, 2));
}

sf::Vector2f vecUnit(sf::Vector2f v) {
	float len = vecLength(v);
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

void projPoly(const std::vector<sf::Vector2f> &polygon,
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
	return vecLength(A);
}

float distanceLinePoint(sf::Vector2f A, sf::Vector2f B, sf::Vector2f P) {
	return fabs((B.y-A.y)*P.x - (B.x-A.x)*P.y + B.x*A.y - A.x*B.y) / vecLength(B-A);
		//sqrt(powf(B.y-A.y, 2) + powf(B.x-A.x, 2));
}

bool isPointInPoly(sf::Vector2f P, const std::vector<sf::Vector2f> &poly) {
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
	// TODO: can we make this faster?
	sf::Vector2f A_B = A-B;
	sf::Vector2f C_D = C-D;
	return ((crossProduct(A_B, C - B) > 0) != (crossProduct(A_B, D - B) > 0) &&
			(crossProduct(C_D, A - D) > 0) != (crossProduct(C_D, B - D) > 0));
}

sf::Vector2f lineLineIntersectionPoint(Point A, Point B, Point C, Point D) {
	float det = (A.x - B.x) * (C.y - D.y) - (A.y - B.y) * (C.x - D.x);
	return sf::Vector2f(
			((A.x*B.y - A.y*B.x) * (C.x - D.x) - (A.x - B.x) * (C.x*D.y - C.y*D.x)) / det,
			((A.x*B.y - A.y*B.x) * (C.y - D.y) - (A.y - B.y) * (C.x*D.y - C.y*D.x)) / det);
}

bool lineCrossesPoly(sf::Vector2f A, sf::Vector2f B,
		const std::vector<sf::Vector2f> &poly) {
	// TODO: can we make this faster?
	if (poly.size() < 2) {
		return false;
	}
	// quickly discard with AABB on AABB test
	if (poly.size() >= 4) {
		float top, bot, left, right;
		bool first = true;
		for (auto &point : poly) {
			if (first || point.x > right) right = point.x;
			if (first || point.x < left) left = point.x;
			if (first || point.y < top) top = point.y;
			if (first || point.y > bot) bot = point.y;
			first = false;
		}

		float top2 = A.y;
		float bot2 = B.y;
		float left2 = A.x;
		float right2 = B.x;
		minMax(top2, bot2);
		minMax(left2, right2);
		if (!(bot2 > top && bot > top2 && right2 > left && right > left2)) {
			return false;
		}
	}
	// actual test
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

// miss, intersection point
std::tuple<bool, Point>
castRayOntoPoly(Point orig, Point dest, Polygon &poly) {
	if (poly.size() < 2) {
		return std::tuple<bool, Point>(false, Point());
	}
	std::vector< std::tuple<Point, Point> > lines_crossing;
	for (int i=0; i<(int)poly.size(); i++) {
		Point p1 = poly[i];
		Point p2;
		if (i+1 == (int)poly.size()) {
			p2 = poly[0];
		} else {
			p2 = poly[i+1];
		}
		if (lineCrossesLine(orig, dest, p1, p2)) {
			std::tuple<Point, Point> t(p1, p2);
			lines_crossing.push_back(t);
		}
	}
	if (lines_crossing.empty()) {
		return std::tuple<bool, Point>(false, Point(0, 0));
	}
	float mindist = -1.f;
	Point closest;
	for (auto &line : lines_crossing) {
		Point p1;
		Point p2;
		std::tie(p1, p2) = line;
		Point c = lineLineIntersectionPoint(p1, p2, orig, dest);
		float dist = distancePointPoint(orig, c);
		if (mindist == -1.f || dist < mindist) {
			mindist = dist;
			closest = c;
		}
	}
	return std::tuple<bool, Point>(true, closest);
}

bool circleCrossingPolygonAxis(sf::Vector2f point, float radius,
		const std::vector<sf::Vector2f> &polygon) {
	if (polygon.size() < 2) {
		return false;
	}
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

bool lineCrossesCircle(Point A, Point B, Point C, float r) {
	sf::Vector2f AC = C-A;
	sf::Vector2f AB = B-A;
	if (vecLength(AB) <= 0.000f) {
		return vecLength(AC) <= r;
	}
	float proj_d = dotProduct(AC, vecUnit(AB));
	if (!between(proj_d, 0, vecLength(AB))) {
		return false;
	}
	Point D = A + vecUnit(AB) * proj_d;
	return vecLength(D - C) <= r;

	//sf::Vector2f d = B - A;
	//sf::Vector2f f = A - C;
	//if (vecLength(d) <= 0.000f) {
	//	return vecLength(f) <= r;
	//}

	//float a = dotProduct(d, d);
	//float b = 2*dotProduct(f, d);
	//float c = dotProduct(f, f) - r*r;
	//float dis = b * b - 4 * a * c;
	//return dis >= 0;
}

