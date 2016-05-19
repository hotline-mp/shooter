#include "Game.hpp"
#include <iostream>
#include <vector>
#include "KeyNames.hpp"


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
		text.setString("");
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
				next_game_state = previous_game_state.back();
				previous_game_state.pop_back();
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

