#include "Game.hpp"
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include "vector.hpp"

Game::Game() : player(&clock, &window, &camera) {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

    window.create(sf::VideoMode(screen_w, screen_h), "shooter demo",
			//sf::Style::Titlebar | sf::Style::Close);
			sf::Style::Fullscreen,
			settings);
	sf::Vector2u window_size = window.getSize();
	sf::View view(sf::FloatRect(0, 0, screen_w, screen_h));
	if (window_size.x > window_size.y) {
		float scale_ratio = float(window_size.y) / screen_h;
		float lr = screen_w * scale_ratio / window_size.x;
		view.setViewport(sf::FloatRect(0.5f-lr/2, 0, lr, 1.f));
	} else {
		float scale_ratio = float(window_size.x) / screen_w;
		float lr = screen_h * scale_ratio / window_size.y;
		view.setViewport(sf::FloatRect(0, 0.5f-lr/2, 1.f, lr));
	}
	window.setView(view);
	window.setVerticalSyncEnabled(true);
	window.setMouseCursorVisible(false);
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

	sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
	sf::Vector2f v = window.mapPixelToCoords(pixelPos) - player.position;

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
	//camera = sf::Vector2f(screen_w/2, screen_h/2) - player.position;
	camera = sf::Vector2f(0, 0);
	sf::View view = window.getView();
	view.setCenter(player.position);
	window.setView(view);
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

	sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
	sf::Vector2f mouse_pos = window.mapPixelToCoords(pixelPos);
	//sf::Vector2f mouse_pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
	crosshair.setPosition(mouse_pos);
	window.draw(crosshair);

	for (int i=2; i<5; i++) {
		sf::CircleShape point(1);
		point.setOrigin(1, 1);
		if (i%2) {
			point.setFillColor(sf::Color::Black);
		} else {
			point.setFillColor(sf::Color::White);
		}
		sf::Vector2f v = (mouse_pos - (player.position+camera)) / 5.f * (float)i;
		point.setPosition(player.position+camera+v);
		window.draw(point);
	}

	if (clock.getElapsedTime() < flash_timeout) {
		window.clear(sf::Color::Red);
	}
}

void Game::playingLoop() {
	update();

	// letterbox bg
	window.clear(sf::Color::Black);

	// view bg
	sf::View view = window.getView();
	sf::Vector2f size = view.getSize();
	sf::RectangleShape rs(size);
	rs.setOrigin(size/2.f);
	rs.setPosition(view.getCenter());
	rs.setFillColor(sf::Color(0x59, 0x30, 0x1B));
	window.draw(rs);

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
			window.setMouseCursorVisible(true);
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

	if(!crosshair_texture.loadFromFile("crosshair.png")) {
		exit(1);
	}

	crosshair.setTexture(crosshair_texture);
	sf::Vector2u crosshair_size = crosshair_texture.getSize();
	crosshair.setOrigin(crosshair_size.x/2, crosshair_size.y/2);

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

