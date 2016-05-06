#include "Game.hpp"
#include "vector.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>

void Game::mapEditorLoop() {
	sf::Time time_now = clock.getElapsedTime();
	if (lastFrame.asMicroseconds() == 0) {
		lastFrame = time_now;
		return;
	}
	long long micros = time_now.asMicroseconds() - lastFrame.asMicroseconds();
	lastFrame = time_now;
	const float vel = 0.0003; // pixels / ms

	updateDirection();
	camera += -player.moving * float(micros * vel);
	window.clear(sf::Color::Cyan);

	text.setString("map editor. press esc to exit");
	text.setPosition(0, 300);
	window.draw(text);
	text.setString("");
	for (int i=0; i<(int)map.size(); i++) {
		sf::ConvexShape polygon;
		polygon.setPointCount(map[i].size());
		for (int j=0; j<(int)map[i].size(); j++) {
			polygon.setPoint(j, map[i][j] + camera);
		}
		if (i == selected_poly) {
			polygon.setFillColor(sf::Color::Green);
		}
		window.draw(polygon);
	}
	for (int i=0; i<(int)map.size(); i++) {
		for (int j=0; j<(int)map[i].size(); j++) {
			sf::CircleShape point(3);
			point.setOrigin(3, 3);
			point.setFillColor(sf::Color::Blue);
			if (i == selected_poly && j == selected_point) {
				point.setFillColor(sf::Color::Red);
			}
			point.setPosition(map[i][j] + camera);
			window.draw(point);
		}
	}
	for (auto &enemy : enemies) {
		enemy.draw();
	}
	if (selected_enemy != -1) {
			sf::CircleShape point(3);
			point.setOrigin(3, 3);
			point.setFillColor(sf::Color::Yellow);
			point.setPosition(enemies[selected_enemy].position + camera);
			window.draw(point);
	}
}

void Game::mapEditorHandleEvent(sf::Event &event) {
	sf::Vector2f mouse_coords(sf::Mouse::getPosition(window));
	mouse_coords -= camera;
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Escape) {
			next_game_state = Playing;
			std::cout << "exitmapedit" << std::endl;
		}
		if (event.key.code == sf::Keyboard::Num1) {
			enemies.push_back(Enemy(&textures[0], &clock, &window, &camera));
			enemies[enemies.size()-1].position = mouse_coords;
		}
		if (event.key.code == sf::Keyboard::Delete) {
			if (selected_enemy != -1) {
				enemies.erase(enemies.begin() + selected_enemy);
				selected_enemy = -1;
			} else if (selected_poly != -1) {
				if (selected_point != -1) {
					map[selected_poly].erase(map[selected_poly].begin() + selected_point);
					selected_point = -1;
				} else {
					map.erase(map.begin() + selected_poly);
					selected_poly = -1;
				}
			}
		}
		if (event.key.code == sf::Keyboard::F1) { // save
			std::ofstream out;
			out.open("map1.dat", std::ios::out | std::ios::trunc | std::ios::binary);
			out << "MAP";
			if (!out.good()) {
				std::cout << "error opening file" << std::endl;
				exit(1);
			}
			for (int i=0; i<(int)map.size(); i++) {
				out << 'P';
				for (int j=0; j<(int)map[i].size(); j++) {
					out << 'p';
					out << 'x';
					int32_t intx = int32_t(map[i][j].x);
					out.write((char*)&intx, sizeof(int32_t));
					out << 'y';
					int32_t inty = int32_t(map[i][j].y);
					out.write((char*)&inty, sizeof(int32_t));
				}
			}
			out.close();
		}
		if (event.key.code == sf::Keyboard::F2) { // load
			std::ifstream file;
			file.open("map1.dat", std::ios::binary);
			if (!file.good()) {
				std::cout << "error opening file" << std::endl;
				exit(1);
			}
			char head[4];
			file.read(head, 3);
			head[3] = '\0';
			if (std::string(head) != "MAP") {
				std::cout << "bad map" << std::endl;
				std::cout << head;
				exit(1);
			}
			map = std::vector< std::vector<sf::Vector2f> >();
			while (!file.eof()) {
				char c;
				file.read(&c, 1);
				if (c == 'P') {
					map.push_back(std::vector<sf::Vector2f>());
				} else if (c == 'p') {
					if (map.size() == 0) {
						std::cout << "bad map p" << std::endl;
					}
					file.read(&c, 1);
					if (c != 'x') {
						std::cout << "bad map x" << std::endl;
						exit(1);
					}
					int32_t x;
					file.read((char*)&x, sizeof(int32_t));
					file.read(&c, 1);
					if (c != 'y') {
						std::cout << "bad map y" << std::endl;
						exit(1);
					}
					int32_t y;
					file.read((char*)&y, sizeof(int32_t));
					sf::Vector2f p(x, y);
					map[map.size()-1].push_back(p);
				}
			}
			file.close();
		}
	} else if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Right) {
			if (selected_poly == -1) {
				map.push_back(std::vector<sf::Vector2f>{{mouse_coords}});
			} else {
				if (selected_point == -1) {
					map[selected_poly].push_back(mouse_coords);
				} else {
					map[selected_poly][selected_point] = mouse_coords;
				}
			}
        } else if (event.mouseButton.button == sf::Mouse::Left) {
			selected_enemy = -1;
			int previous = selected_point;
			for (int i=0; i<(int)map.size(); i++) {
				for (int j=0; j<(int)map[i].size(); j++) {
					if (distancePointPoint(map[i][j], mouse_coords) < 7) {
						selected_poly = i;
						selected_point = j;
					}
				}
			}
			if (selected_point == previous) {
				selected_point = -1;
			}
			for (int i=0; i<(int)enemies.size(); i++) {
				if (distancePointPoint(enemies[i].position, mouse_coords) < 7) {
					selected_enemy = i;
					selected_poly = -1;
					selected_point = -1;
				}
			}
		}
	}
}


