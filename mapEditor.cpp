#include "Game.hpp"
#include "vector.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>

void Game::deselect_all() {
	selected_points.clear();
	selected_enemies.clear();
    selected_spawn_pos = -1;
    selected_warp_pos = -1;
}

void Game::maybe_deselect_all() {
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
		deselect_all();
	}
}

bool Game::anything_selected() {
	return (selected_enemies.size() != 0 ||
			selected_points.size() != 0 ||
			selected_spawn_pos != -1 ||
			selected_warp_pos != -1);
}

sf::Vector2f get_mouse_coords(sf::RenderWindow &window) {
	sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
	return window.mapPixelToCoords(pixelPos);
}

bool in(std::vector<Enemy*> enemies, Enemy* enemy) {
	return std::find(enemies.begin(), enemies.end(), enemy) !=
		enemies.end();
}

bool in(std::vector<sf::Vector2f*> points, sf::Vector2f* point) {
	return std::find(points.begin(), points.end(), point) !=
		points.end();
}

std::vector<sf::Vector2f> *getParentPoly(std::vector<std::vector<sf::Vector2f>> &map,
		sf::Vector2f *p) {
	for (auto &poly : map) {
		for (auto &point : poly) {
			if (&point == p) {
				return &poly;
			}
		}
	}
	return nullptr;
}

void Game::mapEditorLoop() {
    	sf::View view = window.getView();
	sf::Vector2f size = view.getSize();
	sf::Vector2f position = (size/2.f);
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
	sf::Vector2f mouse_coords = get_mouse_coords(window);
	sf::Vector2f mouse_delta = mouse_coords - last_mouse_coords;
	last_mouse_coords = mouse_coords;
	if (dragging && distancePointPoint(mouse_coords, drag_start_coords) > 5) {
		if (selected_points.size() != 0) {
			for (auto point : selected_points) {
				*point += mouse_delta;
			}
		}

		if (selected_enemies.size() != 0) {
			for (auto enemy : selected_enemies) {
				enemy->position += mouse_delta;
			}
		}

		if (selected_spawn_pos != -1) {
			spawn_pos += mouse_delta;
		}

		if (selected_warp_pos != -1) {
			warp_pos += mouse_delta;
		}
	}

	view.setCenter(view.getCenter() + player.moving * float(micros * vel));
	window.setView(view);

	//draw
	window.clear(sf::Color::Cyan);

    switch (map_n) {
	case 0:
		sf::Vector2f b(-1750, -1095);
		for (int i=0; i<14; i++) {
			map_sprite.setTexture(map_textures[i]);
			map_sprite.setPosition(b+sf::Vector2f(1024*(i%5), 1024*(i/5)));
			window.draw(map_sprite);
		}
		window.draw(map_sprite);
        break;
	}



	for (int i=0; i<(int)map.size(); i++) {
		sf::ConvexShape polygon;
		polygon.setPointCount(map[i].size());
		for (int j=0; j<(int)map[i].size(); j++) {
			polygon.setPoint(j, map[i][j]);
		}
		window.draw(polygon);
	}
	for (int i=0; i<(int)map.size(); i++) {
		for (int j=0; j<(int)map[i].size(); j++) {
			sf::CircleShape point(3);
			point.setOrigin(3, 3);
			point.setFillColor(sf::Color::Green);
			if (in(selected_points, &map[i][j])) {
				point.setFillColor(sf::Color::Red);
			}
			point.setPosition(map[i][j]);
			window.draw(point);
		}
	}
	for (auto &enemy : enemies) {
		enemy.draw();
	}
	for (auto enemy : selected_enemies) {
		sf::CircleShape point(3);
		point.setOrigin(3, 3);
		point.setFillColor(sf::Color::Yellow);
		point.setPosition(enemy->position);
		window.draw(point);
	}

	// draw spawn point
	sf::CircleShape point(10);
	point.setOrigin(10, 10);
	point.setFillColor(sf::Color::Yellow);
	point.setPosition(spawn_pos);
	window.draw(point);
	if (selected_spawn_pos != -1) {
		point.setRadius(3);
		point.setOrigin(3, 3);
		point.setFillColor(sf::Color::Red);
		point.setPosition(spawn_pos);
		window.draw(point);
	}
	// draw warp point
	point.setRadius(10);
	point.setOrigin(10, 10);
	point.setFillColor(sf::Color(0xFF, 0xAA, 0xAA));
	point.setPosition(warp_pos);
	window.draw(point);
	if (selected_warp_pos != -1) {
		point.setRadius(3);
		point.setOrigin(3, 3);
		point.setFillColor(sf::Color::Red);
		point.setPosition(warp_pos);
		window.draw(point);
	}

	// drag
	if (selecting && //!anything_selected() &&
			distancePointPoint(mouse_coords, drag_start_coords) >= 5) {
		sf::RectangleShape rs(mouse_coords - drag_start_coords);
		rs.setPosition(drag_start_coords);
		rs.setFillColor(sf::Color(0, 0, 0, 0));
		rs.setOutlineColor(sf::Color::Blue);
		rs.setOutlineThickness(2);
		window.draw(rs);

		text.setString("lmao");
		text.setPosition(500, 0);
		window.draw(text);
	} else {
		text.setString("ayy");
		text.setPosition(500, 0);
		window.draw(text);
	}

	// draw hud
	view = window.getView();
	// view for hud
	sf::View view0 = view;
	view0.setCenter(view.getSize()/2.f);
	window.setView(view0);

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
		window.draw(text);
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
	// normal view
	window.setView(view);
}

bool testClickedPoint(std::vector<std::vector<sf::Vector2f>> &map,
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

bool testClickedPos(sf::Vector2f pos, sf::Vector2f mouse_coords) {
	return distancePointPoint(pos, mouse_coords) < 7;
}

bool testClickedPoint(std::vector<std::vector<sf::Vector2f>> &map,
		sf::Vector2f mouse_coords, sf::Vector2f *&clicked_point) {
	for (int i=0; i<(int)map.size(); i++) {
		for (int j=0; j<(int)map[i].size(); j++) {
			if (distancePointPoint(map[i][j], mouse_coords) < 7) {
				clicked_point = &map[i][j];
				return true;
			}
		}
	}
	return false;
}

bool testClickedPoly(std::vector<std::vector<sf::Vector2f>> &map,
		sf::Vector2f mouse_coords, std::vector<sf::Vector2f> *&clicked_poly) {
	for (int i=0; i<(int)map.size(); i++) {
		if (isPointInPoly(mouse_coords, map[i])) {
			clicked_poly = &map[i];
			return true;
		}
	}
	return false;
}

bool testClickedEnemy(std::vector<Enemy> &enemies,
		sf::Vector2f mouse_coords, Enemy *&clicked_enemy) {
	int radius = 15;

	clicked_enemy = nullptr;
	for (auto &enemy : enemies) {
		if (distancePointPoint(enemy.position, mouse_coords) < radius) {
			clicked_enemy = &enemy;
			return true;
		}
	}
	return false;
}

void Game::mapEditorHandleEvent(sf::Event &event) {
	sf::Vector2f mouse_coords = get_mouse_coords(window);
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Escape) {
			window.setMouseCursorVisible(false);
			next_game_state = Playing;
			std::cout << "exitmapedit" << std::endl;
		}
		if (event.key.code == sf::Keyboard::Num1) {
			enemies.push_back(Enemy(&textures[0], &zombies_sound, &clock, &window));
			enemies[enemies.size()-1].position = mouse_coords;
		}
		if (event.key.code == sf::Keyboard::Delete) {
			std::vector<Enemy> new_enemies;
			for (auto &enemy : enemies) {
				bool found = false;
				for (auto senemy : selected_enemies) {
					if (&enemy == senemy) {
						found = true;
					}
				}
				if (!found) {
					new_enemies.push_back(enemy);
				}
			}
			enemies = new_enemies;

			selected_enemies.clear();
			for (auto point : selected_points) {
				for (auto &poly : map) {
					poly.erase(std::remove(poly.begin(), poly.end(), *point), poly.end());
				}
			}
			selected_points.clear();
			// remove empty polys
			map.erase(std::remove_if(map.begin(), map.end(),
						[](std::vector<sf::Vector2f> &b) { return b.size() == 0; }), map.end());
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
		} else if (event.key.code == sf::Keyboard::Space) {
            deselect_all();
		}
	} else if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Right) {
			if (selected_points.size() == 0) {
				map.push_back(std::vector<sf::Vector2f>{{mouse_coords}});
				deselect_all();
				auto p = &map[map.size()-1];
				selected_points.push_back(&p->at(p->size()-1));
			} else if (selected_points.size() == 1) {
				std::vector<sf::Vector2f> *poly =
					getParentPoly(map, selected_points[0]);
				auto p = poly->insert(
						poly->begin()+(selected_points[0]-&poly->at(0)), mouse_coords);
				deselect_all();
				selected_points.push_back(&(*p));
			} else {
				error_message_timeout = clock.getElapsedTime() + sf::seconds(2);
				error_message = "Select 0 or 1 point to add points";
			}
        } else if (event.mouseButton.button == sf::Mouse::Left) {
			Enemy *e;
			std::vector<sf::Vector2f> *pl;
			sf::Vector2f *pt;
			dragging = true;
			if (testClickedEnemy(enemies, mouse_coords, e)) {
				if (!in(selected_enemies, e)) {
					maybe_deselect_all();
					selected_enemies.push_back(e);
				}
			} else if (testClickedPoint(map, mouse_coords, pt)) {
				if (!in(selected_points, pt)) {
					maybe_deselect_all();
					selected_points.push_back(pt);
				}
			} else if (testClickedPoly(map, mouse_coords, pl)) {
				bool selected = true;
				for (auto &point : *pl) {
					if (!in(selected_points, &point)) {
						selected = false;
						break;
					}
				}
				if (!selected) {
					maybe_deselect_all();
					for (auto &point : *pl) {
						selected_points.push_back(&point);
					}
				}
			} else if (testClickedPos(spawn_pos, mouse_coords)) {
				if (selected_spawn_pos == -1) {
					maybe_deselect_all();
				}
				selected_spawn_pos = 0;
			} else if (testClickedPos(warp_pos, mouse_coords)) {
				if (selected_warp_pos == -1) {
					maybe_deselect_all();
				}
				selected_warp_pos = 0;
			} else {
				selecting = true;
				dragging = false;
			}
			drag_start_coords = mouse_coords;
			last_mouse_coords = mouse_coords;
		}
	} else if (event.type == sf::Event::MouseButtonReleased) {
		dragging = false;
		if (selecting) {
			maybe_deselect_all();
			sf::Rect<float> rect(drag_start_coords, mouse_coords - drag_start_coords);
			for (auto &enemy : enemies) {
				if (rect.contains(enemy.position)) {
					selected_enemies.push_back(&enemy);
				}
			}
			for (auto &poly : map) {
				for (auto &point : poly) {
					if (rect.contains(point)) {
						selected_points.push_back(&point);
					}
				}
			}
			if (rect.contains(spawn_pos)) {
				selected_spawn_pos = 0;
			}
			if (rect.contains(warp_pos)) {
				selected_warp_pos = 0;
			}
		}
		selecting = false;
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
	map.clear();
	enemies.clear();
	bullets.clear();
	knives.clear();
	magazines.clear();
	particles.clear();
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
			enemies.push_back(Enemy(&textures[0], &zombies_sound, &clock, &window));
			enemies[enemies.size()-1].position = p;
		} else if (c == 's') {
			spawn_pos = read_point(file);
		} else if (c == 'w') {
			warp_pos = read_point(file);
		}
	}
	player.position = spawn_pos;
	file.close();
	return 0;
}

