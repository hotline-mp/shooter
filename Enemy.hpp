#ifndef ENEMY_H
#define ENEMY_H

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
	private:
		sf::Sprite sprite;
		void nextFrame();
		std::vector<sf::IntRect> frames;
		int frame;
		sf::Time lastAnimFrame;
		int enemy_type=rand()%2;
		int rotation_notseen=rand()%360;
		bool seen_player = false;
};

#endif /* !ENEMY_H */
