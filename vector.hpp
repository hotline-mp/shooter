#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <vector>

#include <SFML/System/Vector2.hpp>

sf::Vector2f vecUnit(sf::Vector2f v);
float dotProduct(sf::Vector2f u, sf::Vector2f v);
void minMax(float &min, float &max);
bool between(float n, float a, float b);
float distancePointPoint(sf::Vector2f A, sf::Vector2f B);
float distanceLinePoint(sf::Vector2f A, sf::Vector2f B, sf::Vector2f P);

#endif /* !VECTOR_HPP */
