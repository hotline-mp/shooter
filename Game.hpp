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
		// game_state és la pantalla en la que es troba el motor en aquest moment
		// (Playing, MainMenu, etc.)
		// next_game_state és l'estat en que estarà el següent frame.
		// previous_game_state ens serveix per tornar a l'estat anterior quan
		// entrem al menú d'opcions, al que es pot entrar des del menú principal o
		// des del menú de pausa.
		GameState game_state = MainMenu;
		GameState next_game_state = MainMenu;
		std::vector<GameState> previous_game_state;

		void playingLoop();
		void playingHandleEvent(sf::Event &event);
		void keysMenuLoop();
		void keysMenuHandleEvent(sf::Event &event);
		void mapEditorLoop();
		void mapEditorHandleEvent(sf::Event &event);
		void mainMenuLoop();
		void mainMenuHandleEvent(sf::Event &event);
		void pauseMenuLoop();
		void pauseMenuHandleEvent(sf::Event &event);

		Menu keysMenu;
		Menu mainMenu;
		Menu pauseMenu;

		void draw();

		void update();
		void updateDirection();
		sf::Vector2f bulletSpawnPosition();

		int getConfig();
		int saveConfig();
		libconfig::Config cfg;
		std::unordered_map<std::string, int> keys;

		int loadMap(std::string name);
		int loadMap(int n);
		int saveMap(std::string name);
		int saveMap(int n);

		sf::Font font;
		sf::Text text;

		// for key config
		bool waiting_for_input = false;

		std::vector< std::vector<sf::Vector2f> > map;

		std::vector<Particle> particles;
		std::vector<Bullet> bullets;
		std::vector<Enemy> enemies;
		std::vector<sf::Texture> textures;

		sf::Clock clock;

		Player player;

		bool dbg_enabled = false;

		sf::Time flash_timeout;

		sf::Time lastFrame;

		int map_n = 0;

		sf::Vector2f spawn_pos;
		sf::Vector2f warp_pos;

		// crosshair

		sf::Texture crosshair_texture;
		sf::Sprite crosshair;

		// editor
		bool show_editor_help = true;
		int selected_poly = -1;
		int selected_point = -1;
		int selected_enemy = -1;

		int selected_spawn_pos = -1;
		int selected_warp_pos = -1;

		void deselect_all();

		std::string error_message = "";
		sf::Time error_message_timeout;

		bool clicked_on_already_selected_point = false;
		bool dragging = false;
		sf::Vector2f drag_start_coords;

	public:
		Game();
		int run();
		sf::RenderWindow window;
};

#endif /* !GAME_H */
