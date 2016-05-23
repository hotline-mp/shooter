#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.hpp"

const int mag_size = 7;
class Player : public Entity
{
	public:
		Player(sf::Clock *clock, sf::RenderWindow *window);
		void draw();
		void update();
		void hit(sf::Vector2f direction);
		int ammo;
		int extra_ammo;
		int hp;
		bool reloading;
		sf::Time reload_timer;
	private:
		//sf::CircleShape shape{player_r};
		sf::Texture legs;
		sf::Texture body;
		sf::Sprite piernas;
		sf::Sprite torso;
		void nextFrame();
		std::vector<sf::IntRect> frames;
		int frame;
		sf::Time lastAnimFrame;
		//sf::CircleShape shape;
};

#endif /* !PLAYER_H */
