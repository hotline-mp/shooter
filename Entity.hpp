#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

class Entity
{
	public:
		sf::Vector2f position;
		sf::Vector2f moving; // direcció en la que s'intenta moure
		sf::Vector2f facing; // vector unitari
		bool alive;
		void setPosition(float x, float y);
		void setMoving(float x, float y);
		void setFacing(float x, float y);
		void draw(sf::RenderWindow &windows);
};

#endif /* !ENTITY_H */
