#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <vector>
#include <tuple>

#include <SFML/System/Vector2.hpp>

typedef sf::Vector2f Point;
typedef std::vector<sf::Vector2f> Polygon;
typedef std::vector<std::vector<sf::Vector2f> > Map;

float vecLength(sf::Vector2f v);
sf::Vector2f vecUnit(sf::Vector2f v);
float dotProduct(sf::Vector2f u, sf::Vector2f v);
void minMax(float &min, float &max);
bool between(float n, float a, float b);
float distancePointPoint(sf::Vector2f A, sf::Vector2f B);
float distanceLinePoint(sf::Vector2f A, sf::Vector2f B, sf::Vector2f P);
bool isPointInPoly(sf::Vector2f P, const std::vector<sf::Vector2f> &poly);
bool lineCrossesPoly(sf::Vector2f A, sf::Vector2f B, const std::vector<sf::Vector2f> &poly);
bool circleCrossingPolygonAxis(sf::Vector2f point, float radius, const std::vector<sf::Vector2f> &polygon);
bool lineCrossesCircle(sf::Vector2f A, sf::Vector2f B, sf::Vector2f C, float r);
bool lineCrossesLine(sf::Vector2f A, sf::Vector2f B, sf::Vector2f C, sf::Vector2f D);
sf::Vector2f lineLineIntersectionPoint(Point A, Point B, Point C, Point D);
std::tuple<bool, Point>
castRayOntoPoly(Point orig, Point dest, Polygon &poly);

#endif /* !VECTOR_HPP */
