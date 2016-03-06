#include "Player.hpp"

Player::Player(sf::Clock &clock, sf::RenderWindow &window, sf::Vector2f &camera) :
	Entity(clock, window, camera)
{
	shape.setFillColor(sf::Color::Green);
}

void Player::draw() {
	shape.setPosition(position + camera);
	window.draw(shape);
}

