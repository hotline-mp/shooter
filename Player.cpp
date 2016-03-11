#include "Player.hpp"

Player::Player(sf::RenderWindow &window, sf::Vector2f &camera) :
	Entity(window, camera)
{
	shape.setFillColor(sf::Color::Green);
}

void Player::draw() {
	shape.setPosition(position + camera);
	window.draw(shape);
}

