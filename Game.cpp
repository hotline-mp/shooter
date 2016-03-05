#include "Game.hpp"
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include "KeyNames.hpp"

using namespace libconfig;

Game::Game() {
    window.create(sf::VideoMode(screen_w, screen_h), "shooter demo",
			sf::Style::Titlebar | sf::Style::Close);
	window.setVerticalSyncEnabled(true);
}

void Game::updateDirection() {
	/*
	 * Depenent de les tecles de direcció que s'estiguin prement
	 * Posar el vector de direcció del jugador a lloc,
	 * assegurant que sempre tingui longitud 1 o 0
	 */
	float x = 0;
	float y = 0;
	const float sinpiquarts = 0.70710678118;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
			sf::Keyboard::isKeyPressed((sf::Keyboard::Key)keys["left"])) {
		x = -1;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
			sf::Keyboard::isKeyPressed((sf::Keyboard::Key)keys["right"])) {
		x = 1;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
			sf::Keyboard::isKeyPressed((sf::Keyboard::Key)keys["up"])) {
		y = -1;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
			sf::Keyboard::isKeyPressed((sf::Keyboard::Key)keys["down"])) {
		y = 1;
	}
	if (x==1 && y==1) {
		x = sinpiquarts;
		y = sinpiquarts;
	} else if (x==1 && y==-1) {
		x = sinpiquarts;
		y = -sinpiquarts;
	} else if (x==-1 && y==1) {
		x = -sinpiquarts;
		y = sinpiquarts;
	} else if (x==-1 && y==-1) {
		x = -sinpiquarts;
		y = -sinpiquarts;
	}
	player.setMoving(x, y);
}

void Game::update() {
	updateDirection();
	sf::Vector2i localPosition = sf::Mouse::getPosition(window);
	sf::Vector2f v(
			localPosition.x - player.position.x - player_r,
			localPosition.y - player.position.y - player_r);
	float len = sqrt(pow(v.x, 2) + pow(v.y, 2));
	player.facing = v / len;
}

const char* cfg_filename = "shooter.cfg";

int Game::getConfig() {
	// default config
	keys["up"] = sf::Keyboard::W;
	keys["down"] = sf::Keyboard::S;
	keys["left"] = sf::Keyboard::A;
	keys["right"] = sf::Keyboard::D;

	try
	{
		cfg.readFile(cfg_filename);
	}
	catch(const FileIOException &fioex)
	{
		/* el fitxer no existia, probablement per que es la primera
		 * vegada que es juga, ignorar */
		return 1;
	}
	catch(const ParseException &pex)
	{
		// TODO: error grafic
		std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
			<< " - " << pex.getError() << std::endl;
		return 1;
	}

	std::vector<std::string> to_read {"up", "down", "left", "right"};
	for (std::string key : to_read) {
		try {
			keys[key] = cfg.lookup(key);
		} catch(const SettingNotFoundException &nfex) {
			/* si no esta a la config no pasa res,
			 * ja hem definit el valor per defecte abans */
		}
	}
	return 0;
}

int Game::saveConfig() {
	Setting &root = cfg.getRoot();
	for (auto& kv : keys) {
		if (root.exists(kv.first)) {
			root.remove(kv.first);
		}
		root.add(kv.first, Setting::TypeInt) = kv.second;
	}

	try
	{
		cfg.writeFile(cfg_filename);
	}
	catch(const FileIOException &fioex)
	{
		// TODO: error grafic
		std::cerr << "I/O error while writing file: " << cfg_filename << std::endl;
		return 1;
	}
	return 0;
}

void Game::playingLoop() {
	update();

	const float dbg_r = 5.f;
    sf::CircleShape moving_dbg(dbg_r);
    sf::CircleShape facing_dbg(dbg_r);
    moving_dbg.setFillColor(sf::Color::Blue);
    facing_dbg.setFillColor(sf::Color::Red);

	moving_dbg.setPosition(
			player.position.x + player_r + player.moving.x * player_r - dbg_r,
			player.position.y + player_r + player.moving.y * player_r - dbg_r
			);
	facing_dbg.setPosition(
			player.position.x + player_r + player.facing.x * player_r - dbg_r,
			player.position.y + player_r + player.facing.y * player_r - dbg_r
			);

	window.clear(sf::Color::Cyan);
	//text.setString("hullo " + std::to_string(player.position.y));
	//window.draw(text);
	player.draw(window);
	window.draw(moving_dbg);
	window.draw(facing_dbg);
}

void Game::playingHandleEvent(sf::Event &event) {
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Escape) {
			next_game_state = KeysMenu;
			std::cout << "menu" << std::endl;
		}
	}
}

void Game::keysMenuLoop() {
	keysMenu.values = {
		keyNames[keys["up"]],
		keyNames[keys["down"]],
		keyNames[keys["left"]],
		keyNames[keys["right"]]
	};

	window.clear(sf::Color::Cyan);
	keysMenu.draw(window);
	if (waiting_for_input) {
		text.setString("Press a key...");
		sf::FloatRect r = text.getLocalBounds();
		text.setPosition((screen_w - r.width) / 2,
				(screen_h - r.height) / 2);
		window.draw(text);
	}
}

void Game::keysMenuHandleEvent(sf::Event &event) {
	if (event.type == sf::Event::KeyPressed) {
		if (waiting_for_input) {
			std::vector<std::string> ikeys = {
				"up",
				"down",
				"left",
				"right"
			};
			keys[ikeys[keysMenu.selected]] = event.key.code;
			std::cout << "key " << keysMenu.items[keysMenu.selected] << "= "
				<< event.key.code << std::endl;
			waiting_for_input = false;
		} else {
			if (event.key.code == sf::Keyboard::Escape) {
				next_game_state = Playing;
				std::cout << "exitmenu" << std::endl;
			}
			if (event.key.code == sf::Keyboard::Down) {
				keysMenu.selected++;
				if (keysMenu.selected >= (int)keysMenu.items.size()) {
					keysMenu.selected = 0;
				}
			}
			if (event.key.code == sf::Keyboard::Up) {
				keysMenu.selected--;
				if (keysMenu.selected < 0) {
					keysMenu.selected = keysMenu.items.size() - 1;
				}
			}
			if (event.key.code == sf::Keyboard::Return) {
				waiting_for_input = true;
			}
		}
	}
}

int Game::run() {
	getConfig();

	keysMenu.title = "Keys Menu";
	keysMenu.items = {
		"up",
		"down",
		"left",
		"right"
	};

	if (!font.loadFromFile("sans.ttf")) {
		std::cerr << "Could not load font" << std::endl;
		return 1;
	}
	text.setFont(font);
	text.setCharacterSize(24);
	text.setColor(sf::Color::Black);

	player.setPosition(screen_w/2-player_r, screen_h/2-player_r);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
			else {
				switch (game_state) {
					case Playing:
						playingHandleEvent(event);
						break;
					case KeysMenu:
						keysMenuHandleEvent(event);
						break;
					default:
						std::cerr << "error" << std::endl;
						window.close();
				}
			}
        }
		game_state = next_game_state;

		switch (game_state) {
			case Playing:
				playingLoop();
				break;
			case KeysMenu:
				keysMenuLoop();
				break;
			default:
				std::cerr << "error" << std::endl;
				window.close();
		}

        window.display();
    }
	saveConfig();

	return 0;
}

