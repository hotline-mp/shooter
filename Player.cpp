#include "Player.hpp"

Player::Player() {
	shape.setFillColor(sf::Color::Green);
}

void Player::draw(sf::RenderWindow &window) {
	shape.setPosition(position);
	window.draw(shape);
}

