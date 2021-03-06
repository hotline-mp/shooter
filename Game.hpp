#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Clock.hpp>

#include <libconfig.h++>

#include <string>
#include <unordered_map>
#include <vector>
#include <utility>

#include "Player.hpp"
#include "GameState.hpp"
#include "Menu.hpp"
#include "Particle.hpp"
#include "Bullet.hpp"
#include "Enemy.hpp"
#include "Grenade.hpp"
#include "Knife.hpp"
#include "Magazine.hpp"
#include "vector.hpp"

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
		void gameOverLoop();
		void gameOverHandleEvent(sf::Event &event);
		void optionsMenuLoop();
		void optionsMenuHandleEvent(sf::Event &event);

		void changeVolume(int a);
		void setVolumes();

		Menu keysMenu;
		Menu mainMenu;
		Menu pauseMenu;
		Menu optionsMenu;

		void reset();

		void draw();

		void update();
		void updateDirection();
		sf::Vector2f bulletSpawnPosition(bool &possible);

		void splashBlood(Enemy &enemy, sf::Vector2f vel, int amount=40);

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

		std::vector<sf::Texture> textures;
		std::vector<Particle> particles;
		std::vector<Bullet> bullets;
		std::vector<Enemy> enemies;
		std::vector<Grenade> grenades;
		std::vector<Knife> knives;
		std::vector<Magazine> magazines;

		sf::Clock clock;

		Player player;

		bool dbg_enabled = false;

		sf::Time flash_timeout;
		sf::Time input_timeout;

		sf::Time lastFrame;

		int map_n = 0;

		sf::Vector2f spawn_pos;
		sf::Vector2f warp_pos;

		// crosshair

		sf::Texture crosshair_texture;
		sf::Sprite crosshair;

		// audio
		sf::SoundBuffer gunshot_sample;
		sf::Sound gunshot_sound;
		sf::SoundBuffer reload_sample;
		sf::Sound reload_sound;
		sf::SoundBuffer knife_sample;
		sf::Sound knife_sound;
		sf::SoundBuffer game_over_sample;
		sf::Sound game_over_sound;
		sf::SoundBuffer menu_choose_sample;
		sf::Sound menu_choose_sound;
		sf::SoundBuffer hurt_sample;
		sf::Sound hurt_sound;
		sf::SoundBuffer death_scream_sample;
		sf::Sound death_scream_sound;
        sf::SoundBuffer zombies_sample;
        sf::Sound zombies_sound;
		sf::SoundBuffer zombie_death_sample;
		sf::Sound zombie_death_sound;


		sf::Music music;
        sf::Music mainMenu_music;
        sf::Music gameOver_music;
        sf::Music victory_music;

        sf::Texture mainMenu_texture;
        sf::Sprite mainMenu_picture;
        sf::Texture optionMenu_texture;
        sf::Sprite optionMenu_picture;
        sf::Texture pauseMenu_texture;
        sf::Sprite pauseMenu_picture;
        sf::Texture Gameover_texture;
        sf::Sprite Gameover_picture;
        sf::Texture Victory_texture;
        sf::Sprite Victory_picture;

        sf::Texture Map1_texture;
        sf::Sprite Map1_picture;

		std::vector<sf::Texture> map_textures;
        sf::Sprite map_sprite;
		int bg_width = 5;
		sf::Vector2f bg_offset = sf::Vector2f(0, 0);

        int sfx_volume = 20;
        int music_volume = 30;


		// editor
		bool show_editor_help = true;

		std::vector<std::pair<Map::iterator, Polygon::iterator> > selected_points;
		std::vector<std::vector<Enemy>::iterator> selected_enemies;

		int selected_spawn_pos = -1;
		int selected_warp_pos = -1;

		void select_all();
		void deselect_all();
		void maybe_deselect_all();
		bool anything_selected();

		std::string error_message = "";
		sf::Time error_message_timeout;

		bool clicked_on_already_selected_point = false;
		bool dragging = false;
		bool selecting = false;
		sf::Vector2f drag_start_coords;
		sf::Vector2f last_mouse_coords;

		sf::Time second_timer;
		int fps_count = 0;
		int last_fps_count = 0;
		sf::Time spent_in_a;
		sf::Time spent_in_b;
		sf::Time last_spent_in_a;
		sf::Time last_spent_in_b;
		sf::Time frame_timer;

	public:
		Game();
		int run();
		sf::RenderWindow window;
};

#endif /* !GAME_H */
