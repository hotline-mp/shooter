#include "Game.hpp"
#include <iostream>
#include <vector>
#include "Menu.hpp"


void Game::mainMenuLoop()
{
	sf::View view = window.getView();
	sf::Vector2f size = view.getSize();
	sf::Vector2f position = (size/2.f);
	view.setCenter(size/2.f);
	window.setView(view);

	if(!mainMenu_texture.loadFromFile("Mainmenu.png")){
        exit (1);
	}

    mainMenu_picture.setTexture(mainMenu_texture);
    mainMenu_picture.setOrigin(400,300);
    mainMenu_picture.setPosition(position);

    window.clear(sf::Color(0xCC, 0xFF, 0xEE));
	window.draw(mainMenu_picture);
	mainMenu.draw(window);
}

void Game::mainMenuHandleEvent(sf::Event &event)
{
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Return) {
			if (mainMenu.selected == 0) {
				next_game_state = Playing;
            	music.play();
				std::cout << "exitmenu" << std::endl;
			}
			if (mainMenu.selected == 1) {
				previous_game_state.push_back(MainMenu);
				next_game_state = KeysMenu;
				std::cout << "keysmenu" << std::endl;
			}
			if (mainMenu.selected == 2) {
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
		if (event.key.code == sf::Keyboard::Escape) {
			window.close();
		}
	}
}

