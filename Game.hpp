#ifndef GAME_H
#define GAME_H

#include "Player.hpp"

const int screen_w = 800;
const int screen_h = 600;

class Game
{
	private:
		Player player;
		sf::RenderWindow window;
		void update();
		void updateDirection();
	public:
		Game();
		int run();
};

#endif /* !GAME_H */
