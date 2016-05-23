#ifndef ENTITY_H
#define ENTITY_H

#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

class Entity
{
	protected:
		// All of the objects pointed to here shall outlive the Entity
		sf::Clock *clock;
		sf::RenderWindow *window;
		sf::Time lastUpdated;
		float vel; // pixels / ms
	public:
		sf::Vector2f position;
		sf::Vector2f moving; // direcció en la que s'intenta moure
		sf::Vector2f target_movement;
		sf::Vector2f facing; // vector unitari
		float radius; // pixels
		bool alive;
		bool visible;
		void setPosition(float x, float y);
		void setMoving(float x, float y);
		void setFacing(float x, float y);
		void draw();
		void update();
		void pause();
		void collisions(std::vector<sf::Vector2f> polygon);
		void collisions(std::vector< std::vector<sf::Vector2f> > map);

		Entity(sf::Clock *clock, sf::RenderWindow *window) :
			clock(clock), window(window), vel(0.0003f), radius(30.f),
			alive(true), visible(true) {};
	protected:
		Entity(sf::Clock *clock, sf::RenderWindow *window,
				float vel, float radius) :
			clock(clock), window(window), vel(vel), radius(radius),
			alive(true), visible(true) {};
};

#endif /* !ENTITY_H */
