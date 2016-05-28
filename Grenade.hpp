#ifndef NADE_H
#define NADE_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include "Entity.hpp"

class Grenade : public Entity {
	public:
		sf::Vector2f vvel;
		Grenade(sf::Texture *texture, sf::Clock *clock, sf::RenderWindow *window);
		void draw();
		void update(std::vector< std::vector<sf::Vector2f> > map);
	protected:
		sf::Sprite sprite;
		float accel = -0.0000000005f;
};

#endif /* !NADE_H */
