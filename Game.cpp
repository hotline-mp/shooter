#include "Game.hpp"
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include "vector.hpp"

Game::Game() : player(&clock, &window) {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

    window.create(sf::VideoMode(screen_w, screen_h), "shooter demo",
			//sf::Style::Titlebar | sf::Style::Close,
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

	srand(time(NULL));
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

void Game::splashBlood(Enemy &enemy, sf::Vector2f vel, int amount) {
	for (int i=-amount/2; i<amount/2; i++) {
		sf::Vector2f pvel = vecUnit(vecUnit(vel) + vecUnit(sf::Vector2f(vel.y, -vel.x))*float(i/50.0)) *
			(0.0004f + (rand() % 10) / 50000.f);
		Particle part(&clock, &window, rand() % 2 + 1, pvel, -0.0000000005, sf::Color::Red);
		part.position = enemy.position;
		particles.push_back(part);
	}
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

	// get hurt!
	for (auto &enemy : enemies) {
        enemy.update(player, map);
		enemy.collisions(map);
		if (distancePointPoint(enemy.position, player.position) < 30.f) {

			enemy.attacking_timeout = clock.getElapsedTime() + sf::milliseconds(250);

			player.hit(player.position - enemy.position);
			flash_timeout = clock.getElapsedTime() + sf::milliseconds(40);
			player.hp -= 20;
			if (player.hp <= 0) {
				next_game_state = GameOver;
				input_timeout = clock.getElapsedTime() + sf::milliseconds(1000);
				game_over_sound.play();
			}
		}
	}

	player.update();
	player.collisions(map);

	for (auto &bullet : bullets) {
        bullet.update(map);
		for (auto &enemy : enemies) {

			if (std::find(
						bullet.enemies_hit.begin(),
						bullet.enemies_hit.end(),
						&enemy) != bullet.enemies_hit.end()) {
				continue;
			}
			if (distancePointPoint(bullet.position, enemy.position) < enemy.radius) {
				bullet.enemies_hit.push_back(&enemy);
				if (rand() % 3 == 0) {
					splashBlood(enemy, bullet.vvel, 10);
					enemy.stagger_timeout = clock.getElapsedTime() + sf::milliseconds(250);
				} else {
					enemy.alive = false;
					//bullet.alive = false;

					splashBlood(enemy, bullet.vvel);
					if (rand() % 10 == 0) {
						Magazine mag(&textures[3], &clock, &window, enemy.position);
						magazines.push_back(mag);
					}
				}
			}
		}
	}

	for (auto &part : particles) {
        part.update(map);
	}

	for (auto &knife : knives) {
        knife.update(map);
		if (distancePointPoint(knife.position, player.position) < player.radius + 3) {
			knife.alive = false;
		}
		for (auto &enemy : enemies) {
			if (knife.alive && knife.vvel.x != 0 && knife.vvel.y != 0 &&
					distancePointPoint(knife.position, enemy.position) < enemy.radius) {
				enemy.alive = false;
				splashBlood(enemy, knife.vvel);

				if (rand() % 6 == 0) {
					Magazine mag(&textures[3], &clock, &window, enemy.position);
					magazines.push_back(mag);
				}
			}
		}
	}

	for (auto &mag : magazines) {
		if (distancePointPoint(mag.position, player.position) < player.radius + 3) {
			mag.alive = false;
			player.extra_ammo += mag_size;
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
	}

	// el jugador al centre de la pantalla
	sf::View view = window.getView();
	view.setCenter(player.position);
	window.setView(view);
}

void Game::draw() {
//	text.setPosition(0, 300);
//	text.setString("hullo " + std::to_string(player.position.y));
//	window.draw(text);
	for (Knife &knife : knives) {
        knife.draw();
	}
	for (auto &points : map) {
		sf::ConvexShape polygon;
		polygon.setPointCount(points.size());
		int i=0;
		for (auto &point : points) {
			polygon.setPoint(i, point);
			i++;
		}
		polygon.setFillColor(sf::Color::Black);
		window.draw(polygon);
	}
	for (Magazine &mag : magazines) {
        mag.draw();
	}
	player.draw();
	for (Bullet &bullet : bullets) {
        bullet.draw();
	}
	for (Particle &part : particles) {
        part.draw();
	}
	for (Enemy &enemy : enemies) {
        enemy.draw();
	}
	if (enemies.size() == 0) {
		sf::CircleShape point(10);
		point.setOrigin(10, 10);
		point.setFillColor(sf::Color::Green);
		point.setPosition(warp_pos);
		window.draw(point);
	}

	// crosshair
	sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
	sf::Vector2f mouse_pos = window.mapPixelToCoords(pixelPos);
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
		sf::Vector2f v = (mouse_pos - (player.position)) / 5.f * (float)i;
		point.setPosition(player.position+v);
		window.draw(point);
	}

	// hud
	sf::View view = window.getView();
	sf::View view0 = view;
	view0.setCenter(view.getSize()/2.f);
	window.setView(view0);

	// hp
	text.setPosition(10, screen_h*16/20);
	char buf[50];
	sprintf(buf, "health: %d / 100 ", player.hp);
	text.setString(buf);
	text.setColor(sf::Color(0xA6, 0x0B, 0));
	window.draw(text);

	// ammo
	text.setPosition(10, screen_h*17/20);
	sprintf(buf, "ammo: %d / %d %s", player.ammo, player.extra_ammo,
			player.reloading ? "reloading..." : "");
	text.setString(buf);
	text.setColor(sf::Color(0xAA, 0x33, 0));
	window.draw(text);

	window.setView(view);

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
	//rs.setFillColor(sf::Color(0x59, 0x30, 0x1B));
	rs.setFillColor(sf::Color::White);
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
				vdbg_r);
		facing_dbg.setPosition(player.position +
				player.facing * player.radius - vdbg_r);
		window.draw(moving_dbg);
		window.draw(facing_dbg);
	}

	bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
				[](Bullet &b) { return !b.alive; }), bullets.end());
	enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
				[](Enemy &b) { return !b.alive; }), enemies.end());
	particles.erase(std::remove_if(particles.begin(), particles.end(),
				[](Particle &b) { return !b.alive; }), particles.end());
	knives.erase(std::remove_if(knives.begin(), knives.end(),
				[](Knife &b) { return !b.alive; }), knives.end());
	magazines.erase(std::remove_if(magazines.begin(), magazines.end(),
				[](Magazine &b) { return !b.alive; }), magazines.end());
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
		} else if (event.key.code == (sf::Keyboard::Key)keys["reload"] &&
				player.extra_ammo > 0) {
			player.reloading = true;
			player.reload_timer = clock.getElapsedTime() + sf::milliseconds(1000);
			reload_sound.play();
		}
	} else if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
			if (player.ammo > 0 && !player.reloading) {
				player.ammo -= 1;
				bool possible;
				sf::Vector2f pos = bulletSpawnPosition(possible);
				if (!possible) {
					return;
				}
				sf::Vector2f vel = vecUnit(player.moving * player.vel +
						(vecUnit(pos - player.position) * 0.002f)) * 0.002f;
				//sf::Vector2f vel = vecUnit(pos - player.position) * 0.002f;
				Bullet bullet(&clock, &window, vel);
				bullet.position = pos;
				bullets.push_back(bullet);
				gunshot_sound.play();
				for (int i=-10; i<10; i++) {
					sf::Vector2f pvel = vecUnit(vecUnit(vel) + vecUnit(sf::Vector2f(vel.y, -vel.x))*float(i/50.0)) *
						(0.0012f + (rand() % 10) / 100000.f);
					Particle part(&clock, &window, 1, pvel, -0.000000005);
					part.position = bullet.position;
					particles.push_back(part);
				}
			}
        } else if (event.mouseButton.button == sf::Mouse::Right) {
			if (knives.size() == 0) {
				bool possible;
				sf::Vector2f pos = bulletSpawnPosition(possible);
				if (!possible) {
					return;
				}
				sf::Vector2f vel = vecUnit(player.moving * player.vel +
						(vecUnit(pos - player.position) * 0.002f)) * 0.002f;
				Knife knife(&textures[2], &clock, &window, vel);
				knife.position = pos;
				knives.push_back(knife);
				knife_sound.play();
			}
		}
	}
}

void Game::gameOverHandleEvent(sf::Event &event) {
	if (!window.hasFocus()) {
		return;
	}
	if (clock.getElapsedTime() > input_timeout && event.type == sf::Event::KeyPressed) {
		reset();
		next_game_state = MainMenu;
	}
}

void Game::gameOverLoop() {
	window.clear(sf::Color::Red);
	sf::View view = window.getView();
	sf::View view0 = view;
	view0.setCenter(view.getSize()/2.f);
	window.setView(view0);
	text.setPosition(330, screen_h*45/100);
	text.setString("GAME OVER");
	text.setColor(sf::Color(0, 0, 0));
	window.draw(text);
	if (clock.getElapsedTime() > input_timeout) {
		text.setPosition(270, screen_h*55/100);
		text.setString("Press any key to continue...");
		text.setColor(sf::Color(0, 0, 0));
		window.draw(text);
	}
	window.setView(view);
}

void Game::reset() {
	player.reset();
	map_n = 0;
	loadMap(map_n);
}

int Game::run() {
	getConfig();

	mainMenu.title = "";
	mainMenu.items = {
        "Play",
		"Options",
		"Exit"
	};

	pauseMenu.title = "";
	pauseMenu.items = {
        "Resume",
		"Options",
		"Exit"
	};

	keysMenu.title = "";
	keysMenu.items = {
		"up",
		"down",
		"left",
		"right",
		"reload"
	};

	if (!font.loadFromFile("sans.ttf")) {
		std::cerr << "Could not load font" << std::endl;
		return 1;
	}
	text.setFont(font);
	text.setCharacterSize(24);
	text.setColor(sf::Color::Black);

	textures.push_back(sf::Texture());
	textures[0].loadFromFile("enemy1.png");

	textures.push_back(sf::Texture());
	textures[1].loadFromFile("he_grenade.png");

	textures.push_back(sf::Texture());
	textures[2].loadFromFile("knife.png");

	textures.push_back(sf::Texture());
	textures[3].loadFromFile("magazine.png");

	loadMap(map_n);

	player.position = spawn_pos;

	if (!crosshair_texture.loadFromFile("crosshair.png")) {
		exit(1);
	}

	if (!gunshot_sample.loadFromFile("pistol.wav")) {
		exit(1);
	}
	gunshot_sound.setBuffer(gunshot_sample);
	gunshot_sound.setVolume(sfx_volume);

	if (!reload_sample.loadFromFile("reload.wav")) {
		exit(1);
	}
	reload_sound.setBuffer(reload_sample);
	reload_sound.setVolume(sfx_volume);

	if (!knife_sample.loadFromFile("knife.wav")) {
		exit(1);
	}
	knife_sound.setBuffer(knife_sample);
	knife_sound.setVolume(sfx_volume);

	if (!game_over_sample.loadFromFile("game_over.wav")) {
		exit(1);
	}
	game_over_sound.setBuffer(game_over_sample);
	game_over_sound.setVolume(100);

    if (!menu_choose_sample.loadFromFile("menu_choose.wav")) {
        exit(1);
    }
    menu_choose_sound.setBuffer(menu_choose_sample);
    menu_choose_sound.setVolume(sfx_volume);

	if(!mainMenu_music.openFromFile("re_your_brains.wav"))
                {
                    exit (1);
                }
    mainMenu_music.setLoop(true);
    mainMenu_music.setVolume(music_volume);
    mainMenu_music.play();


	if (!music.openFromFile("music.ogg"))
		exit(1);
	music.setLoop(true);
	music.setVolume(music_volume);
	music.play();


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
						menu_choose_sound.play();
						break;
					case MapEditor:
						mapEditorHandleEvent(event);
						break;
                    case MainMenu:
                        mainMenuHandleEvent(event);
                        menu_choose_sound.play();
                        break;
                    case PauseMenu:
                        pauseMenuHandleEvent(event);
                        menu_choose_sound.play();
                        break;
                    case GameOver:
                        gameOverHandleEvent(event);
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
			    mainMenu_music.stop();
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
			case GameOver:
				gameOverLoop();
				mainMenu_music.play();
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

sf::Vector2f Game::bulletSpawnPosition(bool &possible){
	possible = true;
	sf::Vector2f pos = player.position + player.facing * (player.radius * 2);
	for (auto &polygon : map) {
		if (isPointInPoly(pos, polygon)) {
			possible = false;
			break;
		}
	}
    return pos;
}

