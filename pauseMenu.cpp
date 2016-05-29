#include "Game.hpp"
#include <iostream>
#include <vector>
#include "Menu.hpp"


void Game::pauseMenuLoop()
{
	sf::View view = window.getView();
	sf::Vector2f size = view.getSize();
	sf::Vector2f position = (size/2.f);
	view.setCenter(size/2.f);
	window.setView(view);

    if(!pauseMenu_texture.loadFromFile("Pausemenu.png")){
        exit (1);
	}

	pauseMenu_picture.setTexture(pauseMenu_texture);
    pauseMenu_picture.setOrigin(400,300);
    pauseMenu_picture.setPosition(position);

	window.clear(sf::Color(0xCC, 0xFF, 0xEE));
    window.draw(pauseMenu_picture);
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

