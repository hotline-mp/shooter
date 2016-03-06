#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.hpp"

const float player_r = 30.f;

class Player : public Entity
{
	public:
		Player(sf::Clock &clock, sf::RenderWindow &window, sf::Vector2f &camera);
		void draw();
	private:
		sf::CircleShape shape{player_r};
};

#endif /* !PLAYER_H */
