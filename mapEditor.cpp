#include "Game.hpp"
#include "vector.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>

void Game::deselect_all(){

    selected_enemy = -1;
    selected_poly = -1;
    selected_point = -1;
    selected_spawn_pos = -1;
    selected_warp_pos = -1;

}

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

	// dragging
	sf::Vector2f mouse_coords(sf::Mouse::getPosition(window));
	mouse_coords -= camera;
	if (selected_poly != -1 && selected_point != -1 && dragging) {
		if (distancePointPoint(mouse_coords, drag_start_coords) > 5) {
			map[selected_poly][selected_point] = mouse_coords;
		}
	}

	if (selected_enemy != -1 && dragging) {
		if (distancePointPoint(mouse_coords, drag_start_coords) > 5) {
			enemies[selected_enemy].position = mouse_coords;
		}
	}

	if (selected_spawn_pos != -1 && dragging) {
		spawn_pos = mouse_coords;
	}

	if (selected_warp_pos != -1 && dragging) {
		warp_pos = mouse_coords;
	}

	camera += -player.moving * float(micros * vel);

	//draw
	window.clear(sf::Color::Cyan);

	if (show_editor_help) {
		text.setString("map editor. press esc to exit");
		text.setPosition(0, 300);
		window.draw(text);
		text.setString("F1 to save");
		text.setPosition(0, 325);
		window.draw(text);
		text.setString("F2 to load");
		text.setPosition(0, 350);
		window.draw(text);
		char txt[200];
		sprintf(txt, "map number: %d (F3/F4 to change)", map_n);
		text.setString(txt);
		//text.setString("map number: " + std::to_string(map_n) + " (F3/F4 to change)");
		text.setPosition(0, 375);
		window.draw(text);
		text.setString("F5 to show/hide help");
		text.setPosition(0, 400);
		text.setString("Right click to add nodes, 1 to add enemies");
		text.setPosition(0, 425);
		window.draw(text);
	}
	if (clock.getElapsedTime() < error_message_timeout) {
		text.setString(error_message);
		text.setPosition(0, 0);
		window.draw(text);
	}
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
			point.setFillColor(sf::Color::Green);
			if (i == selected_poly) {
				point.setFillColor(sf::Color::Blue);
				if (j == selected_point) {
					point.setFillColor(sf::Color::Red);
				}
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
	// draw spawn point
	sf::CircleShape point(10);
	point.setOrigin(10, 10);
	point.setFillColor(sf::Color::Yellow);
	point.setPosition(spawn_pos + camera);
	window.draw(point);
	if (selected_spawn_pos != -1) {
		point.setRadius(3);
		point.setOrigin(3, 3);
		point.setFillColor(sf::Color::Red);
		point.setPosition(spawn_pos + camera);
		window.draw(point);
	}
	// draw warp point
	point.setRadius(10);
	point.setOrigin(10, 10);
	point.setFillColor(sf::Color(0xFF, 0xAA, 0xAA));
	point.setPosition(warp_pos + camera);
	window.draw(point);
	if (selected_warp_pos != -1) {
		point.setRadius(3);
		point.setOrigin(3, 3);
		point.setFillColor(sf::Color::Red);
		point.setPosition(warp_pos + camera);
		window.draw(point);
	}
}

bool testClickedPoint(std::vector<std::vector<sf::Vector2f>> map,
		sf::Vector2f mouse_coords, int &selected_poly, int &selected_point) {
	bool found = false;
	for (int i=0; i<(int)map.size(); i++) {
		for (int j=0; j<(int)map[i].size(); j++) {
			if (distancePointPoint(map[i][j], mouse_coords) < 7) {
				selected_poly = i;
				selected_point = j;
				found = true;
			}
		}
	}
	return found;
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
			if (saveMap(map_n)) exit(1);
		} else if (event.key.code == sf::Keyboard::F2) { // load
			if (int err_n = loadMap(map_n)) {
				error_message_timeout = clock.getElapsedTime() + sf::seconds(2);
				error_message = "Couldn't load map: error "; // + std::to_string(err_n);
				//exit(1);
			}
		} else if (event.key.code == sf::Keyboard::F3) {
			if (map_n > 0) {
				map_n--;
			}
		} else if (event.key.code == sf::Keyboard::F4) {
			map_n++;
		} else if (event.key.code == sf::Keyboard::F5) {
			show_editor_help = !show_editor_help;
		}
	} else if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Right) {
			if (selected_poly == -1) {
				map.push_back(std::vector<sf::Vector2f>{{mouse_coords}});
				selected_poly = map.size() - 1;
			} else {
				if (selected_point == -1) {
					map[selected_poly].push_back(mouse_coords);
				} else {
					auto poly = &map[selected_poly];
					poly->insert(poly->begin()+selected_point, mouse_coords);
				}
			}
        } else if (event.mouseButton.button == sf::Mouse::Left) {
			int previous = selected_point;
            deselect_all();
			drag_start_coords = mouse_coords;
			if (testClickedPoint(map, mouse_coords, selected_poly, selected_point)) {
				clicked_on_already_selected_point = (selected_point == previous);
				dragging = true;
			} else if (distancePointPoint(mouse_coords, spawn_pos) < 7) {
				if (selected_spawn_pos != -1) {
					selected_spawn_pos = -1;
				} else {
					selected_spawn_pos = 0;
					dragging = true;
				}
			} else if (distancePointPoint(mouse_coords, warp_pos) < 7) {
				if (selected_warp_pos != -1) {
					selected_warp_pos = -1;
				} else {
					selected_warp_pos = 0;
					dragging = true;
				}
			} else {
				for (int i=0; i<(int)enemies.size(); i++) {
					if (distancePointPoint(enemies[i].position, mouse_coords) < 7) {
						selected_enemy = i;
						dragging = true;
					}
				}
				if (selected_enemy == -1) {
					bool found = false;
					for (int i=0; i<(int)map.size(); i++) {
						if (isPointInPoly(mouse_coords, map[i])) {
							found = true;
							selected_poly = i;
							break;
						}
					}
				}
			}
		}
	} else if (event.type == sf::Event::MouseButtonReleased) {
		dragging = false;
		if (selected_poly != -1 && selected_point != -1) {
			if (clicked_on_already_selected_point) {
				// de-select
				selected_point = -1;
			}
		}
	}
}

int Game::saveMap(int n) {
	char s[20];
	sprintf(s, "map%d.dat", map_n);
	//std::string s = "map" + std::to_string(n) + ".dat";
	return saveMap(s);
}

void write_pos(sf::Vector2f p, std::ofstream &out) {
	out << 'x';
	int32_t intx = int32_t(p.x);
	out.write((char*)&intx, sizeof(int32_t));
	out << 'y';
	int32_t inty = int32_t(p.y);
	out.write((char*)&inty, sizeof(int32_t));
}

int Game::saveMap(std::string name) {
	std::ofstream out;
	out.open(name, std::ios::out | std::ios::trunc | std::ios::binary);
	out << "MAP";
	if (!out.good()) {
		std::cout << "error opening file" << std::endl;
		return 1;
	}
	for (int i=0; i<(int)map.size(); i++) {
		out << 'P';
		for (int j=0; j<(int)map[i].size(); j++) {
			out << 'p';
			write_pos(map[i][j], out);
		}
	}
	for (int i=0; i<(int)enemies.size(); i++) {
		out << 'e';
		write_pos(enemies[i].position, out);
	}
	out << 's';
	write_pos(spawn_pos, out);
	out << 'w';
	write_pos(warp_pos, out);
	out.close();
	return 0;
}

int Game::loadMap(int n) {
	char s[20];
	sprintf(s, "map%d.dat", map_n);
	//std::string s = "map" + std::to_string(n) + ".dat";
	return loadMap(s);
}

sf::Vector2f read_point(std::ifstream &file, std::string where="map") {
	char c;
	file.read(&c, 1);
	if (c != 'x') {
		std::cout << file.tellg() << std::endl;
		std::cout << "bad " << where << " x: " << (int)c << std::endl;
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
	return p;
}

int Game::loadMap(std::string name) {
	std::ifstream file;
	file.open(name, std::ios::binary);
	if (!file.good()) {
		std::cout << "error opening file" << std::endl;
		return 1;
	}
	char head[4];
	file.read(head, 3);
	head[3] = '\0';
	if (std::string(head) != "MAP") {
		std::cout << "bad map" << std::endl;
		std::cout << head;
		return 2;
	}
	map = std::vector< std::vector<sf::Vector2f> >();
	enemies = std::vector<Enemy>();
	while (true) {
		char c;
		file.read(&c, 1);
		if (file.eof()) {
			break;
		}
		if (c == 'P') {
			map.push_back(std::vector<sf::Vector2f>());
		} else if (c == 'p') {
			if (map.size() == 0) {
				std::cout << "bad map p" << std::endl;
			}
			sf::Vector2f p = read_point(file, "poly point");
			map[map.size()-1].push_back(p);
		} else if (c == 'e') {
			sf::Vector2f p = read_point(file, "enemy");
			enemies.push_back(Enemy(&textures[0], &clock, &window, &camera));
			enemies[enemies.size()-1].position = p;
		} else if (c == 's') {
			spawn_pos = read_point(file);
		} else if (c == 'w') {
			warp_pos = read_point(file);
		}
	}
	file.close();
	return 0;
}
