#include "Bala.hpp"

Bala::Bala(sf::RenderWindow &window, sf::Vector2f &camera) :
	Entity(window, camera)
{
	shape.setFillColor(sf::Color::Black);
}

void Bala::draw() {
	shape.setPosition(position + camera);
	window.draw(shape);
}
