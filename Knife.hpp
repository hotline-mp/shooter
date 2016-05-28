#ifndef KNIFE_H
#define KNIFE_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include "Entity.hpp"

class Knife : public Entity {
	public:
		sf::Vector2f vvel;
		Knife(sf::Texture *texture, sf::Clock *clock, sf::RenderWindow *window, sf::Vector2f vel);
		void draw();
		void update(std::vector< std::vector<sf::Vector2f> > map);
	protected:
		sf::Sprite sprite;
		float accel = -0.0000000005f;
};

#endif /* !KNIFE_H */
