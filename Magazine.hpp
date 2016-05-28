#ifndef MAGAZINE_H
#define MAGAZINE_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include "Entity.hpp"

class Magazine : public Entity {
	public:
		Magazine(sf::Texture *texture, sf::Clock *clock, sf::RenderWindow *window, sf::Vector2f pos);
		void draw();
	protected:
		sf::Sprite sprite;
};

#endif /* !MAGAZINE_H */
