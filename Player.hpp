#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.hpp"

class Player : public Entity
{
	public:
		Player(sf::Clock *clock, sf::RenderWindow *window, sf::Vector2f *camera);
		void draw();
		void update();
	private:
		//sf::CircleShape shape{player_r};
		sf::Texture texture;
		sf::Sprite sprite;
		void nextFrame();
		std::vector<sf::IntRect> frames;
		int frame;
		sf::Time lastAnimFrame;
		sf::CircleShape shape;
};

#endif /* !PLAYER_H */
