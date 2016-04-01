#include <iostream>
#include "Bala.hpp"

Bala::Bala(sf::Clock &clock, sf::RenderWindow &window, sf::Vector2f &camera) :
	Entity(clock, window, camera, 0.0006f)
{
	shape.setFillColor(sf::Color::Black);
}

void Bala::draw() {
	shape.setPosition(position + camera);
	window.draw(shape);
}
