#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.hpp"

class Player : public Entity
{
	public:
		Player(sf::RenderWindow &window, sf::Vector2f &camera);
		void draw();
	private:
		sf::CircleShape shape{player_r};
		sf::Texture texture;
		sf::Sprite sprite;
};

#endif /* !PLAYER_H */
