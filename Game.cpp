#include "Game.hpp"
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include "vector.hpp"

Game::Game() : player(&clock, &window, &camera) {
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
	if (!window.hasFocus()) {
		return;
	}
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

#define COL_BOX 0
void Game::update() {
	updateDirection();
	sf::Vector2f mousePosition(sf::Mouse::getPosition(window));
	sf::Vector2f v(mousePosition - player.position - camera);
	float len = sqrt(powf(v.x, 2) + powf(v.y, 2));
	// vector unitari que va del centre del jugador
	// en direcció a on apuntem
	player.facing = v / len;

	for (auto &enemy : enemies) {
        enemy.update(player, map);
		enemy.collisions(map);
		if (distancePointPoint(enemy.position, player.position) < 30.f) {
			player.hit(player.position - enemy.position);
			flash_timeout = clock.getElapsedTime() + sf::milliseconds(40);
		}
	}

	player.update();
	player.collisions(map);

	for (auto &bullet : bullets) {
        bullet.update(map);
		for (auto &enemy : enemies) {
			if (distancePointPoint(bullet.position, enemy.position) < enemy.radius) {
				enemy.alive = false;
				bullet.alive = false;
			}
		}
	}

	if (enemies.size() == 0 &&
			distancePointPoint(player.position, warp_pos) < 20) {
		map_n += 1;
		if(loadMap(map_n) != 0) {
			// fin
			map_n = 0;
			loadMap(map_n);
		}

		player.position = spawn_pos;
	}

	// La camera ha de tenir el jugador sempre al centre de la pantalla, per tant:
	// pos en pantalla = pos_jugador + camera = centre_pantalla - tamany_jugador
	// camera = centre_pantalla - tamany_jugador - pos_jugador
	camera = sf::Vector2f(screen_w/2, screen_h/2) - player.position;
}

void Game::draw() {
	text.setPosition(0, 300);
//	text.setString("hullo " + std::to_string(player.position.y));
	window.draw(text);
	player.draw();
	for (auto &points : map) {
		sf::ConvexShape polygon;
		polygon.setPointCount(points.size());
		int i=0;
		for (auto &point : points) {
			polygon.setPoint(i, point + camera);
			i++;
		}
		window.draw(polygon);
	}
	for (Bullet &bullet : bullets) {
        bullet.draw();
	}
	for (Enemy &enemy : enemies) {
        enemy.draw();
	}
	if (enemies.size() == 0) {
		sf::CircleShape point(10);
		point.setOrigin(10, 10);
		point.setFillColor(sf::Color::Green);
		point.setPosition(warp_pos+camera);
		window.draw(point);
	}
	if (clock.getElapsedTime() < flash_timeout) {
		window.clear(sf::Color::Red);
	}
}

void Game::playingLoop() {
	update();

	//window.clear(sf::Color::Cyan);
	window.clear(sf::Color(0x59, 0x30, 0x1B));

	if (dbg_enabled) {
		const float dbg_r = 5.f;
		sf::CircleShape moving_dbg(dbg_r);
		sf::CircleShape facing_dbg(dbg_r);

		moving_dbg.setFillColor(sf::Color::Blue);
		facing_dbg.setFillColor(sf::Color::Red);
		sf::Vector2f vdbg_r(dbg_r, dbg_r);
		sf::Vector2f vplayer_r(player.radius, player.radius);
		moving_dbg.setPosition(
				player.position + player.moving * player.radius -
				vdbg_r + camera);
		facing_dbg.setPosition(player.position +
				player.facing * player.radius - vdbg_r + camera);
		window.draw(moving_dbg);
		window.draw(facing_dbg);
	}

	bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
				[](Bullet &b) { return !b.alive; }), bullets.end());
	enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
				[](Enemy &b) { return !b.alive; }), enemies.end());
	draw();
}

void Game::playingHandleEvent(sf::Event &event) {

	if ((event.type == sf::Event::KeyPressed &&
				event.key.code == sf::Keyboard::Escape) ||
			event.type == sf::Event::LostFocus)
	{
			previous_game_state.push_back(Playing);
			next_game_state = PauseMenu;
			std::cout << "menu" << std::endl;
	}

	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Escape) {
			previous_game_state.push_back(Playing);
			next_game_state = PauseMenu;
			std::cout << "menu" << std::endl;
		} else if (event.key.code == sf::Keyboard::F8) {
			next_game_state = MapEditor;
			std::cout << "mapeditor" << std::endl;
		}
	} else if (event.type == sf::Event::MouseButtonPressed){
        if (event.mouseButton.button == sf::Mouse::Left){
            Bullet bullet(&clock, &window, &camera);
            bullet.position = bulletSpawnPosition();
            bullet.moving = vecUnit(bullet.position - player.position);
            bullets.push_back(bullet);
        }
	}
}

int Game::run() {
	getConfig();

	mainMenu.title = "Main Menu";
	mainMenu.items = {
        "Play",
		"Options",
		"Exit"
	};

	pauseMenu.title = "Pause Menu";
	pauseMenu.items = {
        "Resume",
		"Options",
		"Exit"
	};

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

	camera = sf::Vector2f(0, 0);

	textures.push_back(sf::Texture());
	textures[0].loadFromFile("enemy1.png");

	loadMap(map_n);

	player.position = spawn_pos;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
			} else {
				switch (game_state) {
					case Playing:
						playingHandleEvent(event);
						break;
					case KeysMenu:
						keysMenuHandleEvent(event);
						break;
					case MapEditor:
						mapEditorHandleEvent(event);
						break;
                    case MainMenu:
                        mainMenuHandleEvent(event);
                        break;
                    case PauseMenu:
                        pauseMenuHandleEvent(event);
                        break;
					default:
						std::cerr << "error: game state not found" << std::endl;
						window.close();
				}
			}
        }
		if (game_state != next_game_state) {
			for (auto &enemy : enemies) {
				enemy.pause();
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
			case MapEditor:
				mapEditorLoop();
				break;
            case MainMenu:
                mainMenuLoop();
                break;
            case PauseMenu:
                pauseMenuLoop();
                break;
			default:
				std::cerr << "error: game state not found" << std::endl;
				window.close();
		}

        window.display();
    }
	saveConfig();

	return 0;
}

sf::Vector2f Game::bulletSpawnPosition(){
    return (player.position + player.facing * (player.radius * 2));
}

