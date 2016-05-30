#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Audio.hpp>

#include "Entity.hpp"
#include "Player.hpp"

/*
 * Els enemics no guarden les seves propies textures per que estan dins un vector
 * i en copiar-los es trencarien.
 */
class Enemy : public Entity
{
	public:
		Enemy(sf::Texture *texture, sf::Clock *clock, sf::RenderWindow *window);
		void draw();
		//void update();
		void update(Player player, std::vector< std::vector<sf::Vector2f> > map);
		sf::Time stagger_timeout;
		sf::Time attacking_timeout;
	private:
		sf::Sprite sprite;
		void nextFrame();
		std::vector<sf::IntRect> frames;
		int frame;
		sf::Time lastAnimFrame;
		int enemy_type;
		int rotation_notseen=rand()%360;
		bool seen_player = false;
		bool seen_first_time = false;
		sf::Sound zombies_sound;


};

#endif /* !ENEMY_H */
