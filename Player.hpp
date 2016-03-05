#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.hpp"

const float player_r = 30.f;

class Player : public Entity
{
	public:
		Player();
		void draw(sf::RenderWindow &window);
	private:
		sf::CircleShape shape{player_r};
};

#endif /* !PLAYER_H */
