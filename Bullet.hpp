#ifndef BALA_H
#define BALA_H

#include <SFML/Graphics.hpp>
#include "Entity.hpp"

class Bullet : public Entity {
	public:
		void draw();
		Bullet(sf::Clock *clock, sf::RenderWindow *window);
		void update(std::vector< std::vector<sf::Vector2f> > map);
	private:
		sf::CircleShape shape;
};

#endif /* !BALA_H */
