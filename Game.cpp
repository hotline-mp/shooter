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
	sf::Vector2f playerSizeVec(player.radius, player.radius);
	sf::Vector2f v(mousePosition - player.position - playerSizeVec - camera);
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
			if (distancePointPoint(bullet.position, enemy.position) < 30.f) {
				enemy.alive = false;
				bullet.alive = false;
			}
		}
	}

	// La camera ha de tenir el jugador sempre al centre de la pantalla, per tant:
	// pos en pantalla = pos_jugador + camera = centre_pantalla - tamany_jugador
	// camera = centre_pantalla - tamany_jugador - pos_jugador
	camera = sf::Vector2f(screen_w/2, screen_h/2) - playerSizeVec - player.position;
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
				player.position + vplayer_r + player.moving * player.radius -
				vdbg_r + camera);
		facing_dbg.setPosition(player.position + vplayer_r +
				player.facing * player.radius - vdbg_r + camera);
		window.draw(moving_dbg);
		window.draw(facing_dbg);
	}

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
	bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
				[](Bullet &b) { return !b.alive; }), bullets.end());
	for (Bullet &bullet : bullets) {
        bullet.draw();
	}
	enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
				[](Enemy &b) { return !b.alive; }), enemies.end());
	for (Enemy &enemy : enemies) {
        enemy.draw();
	}

	if (clock.getElapsedTime() < flash_timeout) {
		window.clear(sf::Color::Red);
	}
}

void Game::playingHandleEvent(sf::Event &event) {

	if (!window.hasFocus()) {
		return;
	}
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Escape) {
			next_game_state = KeysMenu;
			std::cout << "menu" << std::endl;
		}
	} else if (event.type == sf::Event::MouseButtonPressed){
        if (event.mouseButton.button == sf::Mouse::Left){
            Bullet bullet(&clock, &window, &camera);
			sf::Vector2f player_center = player.position +
				sf::Vector2f(player.radius, player.radius);
            bullet.position = bulletSpawnPosition() - sf::Vector2f(bullet.radius, bullet.radius);
			sf::Vector2f bullet_center = bullet.position +
				sf::Vector2f(bullet.radius, bullet.radius);
            bullet.moving = vecUnit(bullet_center - player_center);
            bullets.push_back(bullet);
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

	camera = sf::Vector2f(0, 0);

	map.resize(2);
	map[0].resize(4);
	map[0][0] = sf::Vector2f(5, 5);
	map[0][1] = sf::Vector2f(560, 5);
	map[0][2] = sf::Vector2f(560, 100);
	map[0][3] = sf::Vector2f(5, 100);
	map[1].resize(3);
	map[1][0] = sf::Vector2f(1000, 5);
	map[1][1] = sf::Vector2f(1560, 5);
	map[1][2] = sf::Vector2f(1560, 100);

	textures.push_back(sf::Texture());
	textures[0].loadFromFile("enemy1.png");
	enemies.push_back(Enemy(&textures[0], &clock, &window, &camera));
	enemies[0].setPosition(50, 200);

	player.setPosition(screen_w/2-player.radius, screen_h/2-player.radius);

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

sf::Vector2f Game::bulletSpawnPosition(){
    sf::Vector2f vplayer_r(player.radius, player.radius);
    return (player.position + vplayer_r + player.facing * player.radius);

}

