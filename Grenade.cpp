#include "Grenade.hpp"

Grenade::Grenade(sf::Texture *texture, sf::Clock *clock, sf::RenderWindow *window) :
	Entity(clock, window)
{
	sprite.setTexture(*texture);
    sprite.setOrigin(15, 15);
}

void Grenade::draw() {
	if (!visible) {
		return;
	}
    sprite.setPosition(position);

	window->draw(sprite);
}


void Grenade::update(std::vector< std::vector<sf::Vector2f> > map) {
}
