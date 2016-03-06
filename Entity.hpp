#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

class Entity
{
	protected:
		sf::Clock &clock;
		sf::RenderWindow &window;
		sf::Vector2f &camera;
		sf::Time lastUpdated;
	public:
		Entity(sf::Clock &clock, sf::RenderWindow &window, sf::Vector2f &camera) :
			clock(clock), window(window), camera(camera) {};

		sf::Vector2f position;
		sf::Vector2f moving; // direcció en la que s'intenta moure
		sf::Vector2f facing; // vector unitari
		bool alive;
		void setPosition(float x, float y);
		void setMoving(float x, float y);
		void setFacing(float x, float y);
		void draw();
		void update();
};

#endif /* !ENTITY_H */
