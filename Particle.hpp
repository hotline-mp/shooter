#ifndef PART_H
#define PART_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include "Entity.hpp"

class Particle : public Entity {
	public:
		void draw();
		sf::Vector2f vvel;
		Particle(sf::Clock *clock, sf::RenderWindow *window, float radius);
		Particle(sf::Clock *clock, sf::RenderWindow *window, float radius,
				sf::Vector2f vel, float accel, sf::Color color=sf::Color::Black);
		void update(std::vector< std::vector<sf::Vector2f> > map);
	protected:
		sf::CircleShape shape;
		float accel;
};

#endif /* !PART_H */
