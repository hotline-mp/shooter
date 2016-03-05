#include "Game.hpp"
#include <iostream>
#include <string>
#include <cmath>

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
			sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		x = -1;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		x = 1;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		y = -1;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
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

int Game::run() {
	sf::Font font; // debugging for now
	sf::Text text;
	if (!font.loadFromFile("sans.ttf")) {
		std::cerr << "Could not load font" << std::endl;
		return 1;
	}
	text.setFont(font);
	text.setCharacterSize(24);
	text.setColor(sf::Color::Black);

	player.setPosition(screen_w/2-player_r, screen_h/2-player_r);

	const float dbg_r = 5.f;
    sf::CircleShape moving_dbg(dbg_r);
    sf::CircleShape facing_dbg(dbg_r);
    moving_dbg.setFillColor(sf::Color::Blue);
    facing_dbg.setFillColor(sf::Color::Red);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

		update();

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
        window.display();
    }

	return 0;
}

