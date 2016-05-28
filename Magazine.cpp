#include "Magazine.hpp"
#include "vector.hpp"
#include <math.h>
#include <iostream>

Magazine::Magazine(sf::Texture *texture, sf::Clock *clock, sf::RenderWindow *window, sf::Vector2f pos) :
	Entity(clock, window)
{
	position = pos;
	sprite.setTexture(*texture);
    sprite.setOrigin(16, 16);
    sprite.setPosition(position);

	radius = 12;
}

void Magazine::draw() {
	if (!visible) {
		return;
	}
	window->draw(sprite);
}

