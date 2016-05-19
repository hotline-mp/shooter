#include "Game.hpp"
#include <iostream>
#include <vector>
#include "Menu.hpp"


void Game::mainMenuLoop()
{
	mainMenu.values = {
		"Play",
		"Options",
		"Exit",
	};

	window.clear(sf::Color::Cyan);
	mainMenu.draw(window);

}

void Game::mainMenuHandleEvent(sf::Event &event)
{
if (event.type == sf::Event::KeyPressed) {
		if (waiting_for_input) {
			std::vector<std::string> ikeys = {
				"Play",
				"Options",
				"Exit",
			};
			keys[ikeys[mainMenu.selected]] = event.key.code;

			waiting_for_input = false;
		} else {
			if (event.key.code == sf::Keyboard::Return) {
				if (mainMenu.selected == 0)
				{
                    next_game_state = Playing;
                    std::cout << "exitmenu" << std::endl;
				}
				if (mainMenu.selected == 1)
				{
                    next_game_state = KeysMenu;
                    std::cout << "exitmenu" << std::endl;
				}
				if (mainMenu.selected == 2)
				{
                    window.close();
				}
			}
			if (event.key.code == sf::Keyboard::Down) {
				mainMenu.selected++;
				if (mainMenu.selected >= (int)mainMenu.items.size()) {
					mainMenu.selected = 0;
				}
			}
			if (event.key.code == sf::Keyboard::Up) {
				mainMenu.selected--;
				if (mainMenu.selected < 0) {
					mainMenu.selected = mainMenu.items.size() - 1;
				}
			}
			if (event.key.code == sf::Keyboard::Return) {
				waiting_for_input = true;
			}
		}
	}
}





