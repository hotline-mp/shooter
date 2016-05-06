#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Clock.hpp>

#include <libconfig.h++>

#include <string>
#include <unordered_map>
#include <vector>

#include "Player.hpp"
#include "GameState.hpp"
#include "Menu.hpp"
#include "Bullet.hpp"
#include "Enemy.hpp"

const int screen_w = 800;
const int screen_h = 600;

class Game
{
	private:
		GameState game_state = Playing;
		GameState next_game_state = Playing;

		void playingLoop();
		void playingHandleEvent(sf::Event &event);
		void keysMenuLoop();
		void keysMenuHandleEvent(sf::Event &event);

		Menu keysMenu;

		sf::RenderWindow window;

		void update();
		void updateDirection();
		sf::Vector2f bulletSpawnPosition();

		int getConfig();
		int saveConfig();
		libconfig::Config cfg;
		std::unordered_map<std::string, int> keys;

		sf::Font font;
		sf::Text text;

		// for key config
		bool waiting_for_input = false;

		std::vector< std::vector<sf::Vector2f> > map;

		std::vector<Bullet> bullets;
		std::vector<Enemy> enemies;
		std::vector<sf::Texture> textures;

		sf::Vector2f camera;

		sf::Clock clock;

		Player player;

		bool dbg_enabled = false;

		sf::Time flash_timeout;

	public:
		Game();
		int run();
};

#endif /* !GAME_H */
