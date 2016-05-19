#include "Game.hpp"
#include <iostream>
#include <vector>
#include "Menu.hpp"


void Game::pauseMenuLoop()
{
	window.clear(sf::Color(0xCC, 0xFF, 0xEE));
	pauseMenu.draw(window);
}

void Game::pauseMenuHandleEvent(sf::Event &event)
{
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Return) {
			if (pauseMenu.selected == 0) {
				next_game_state = Playing;
				std::cout << "exitmenu" << std::endl;
			}
			if (pauseMenu.selected == 1) {
				previous_game_state.push_back(PauseMenu);
				next_game_state = KeysMenu;
				std::cout << "keysmenu" << std::endl;
			}
			if (pauseMenu.selected == 2) {
				window.close();
			}
		}
		if (event.key.code == sf::Keyboard::Down) {
			pauseMenu.selected++;
			if (pauseMenu.selected >= (int)pauseMenu.items.size()) {
				pauseMenu.selected = 0;
			}
		}
		if (event.key.code == sf::Keyboard::Up) {
			pauseMenu.selected--;
			if (pauseMenu.selected < 0) {
				pauseMenu.selected = pauseMenu.items.size() - 1;
			}
		}
		if (event.key.code == sf::Keyboard::Escape) {
			next_game_state = Playing;
			std::cout << "exitmenu" << std::endl;
		}
	}
}

