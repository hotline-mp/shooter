#include "Game.hpp"
#include <iostream>
#include <vector>
#include "KeyNames.hpp"


void Game::keysMenuLoop() {

    sf::View view = window.getView();
	sf::Vector2f size = view.getSize();
	sf::Vector2f position = (size/2.f);

	if(!optionMenu_texture.loadFromFile("Optionmenu.png")){
        exit (1);
	}

	optionMenu_picture.setTexture(optionMenu_texture);
    optionMenu_picture.setOrigin(400,300);
    optionMenu_picture.setPosition(position);

	keysMenu.values = {
		keyNames[keys["up"]],
		keyNames[keys["down"]],
		keyNames[keys["left"]],
		keyNames[keys["right"]],
		keyNames[keys["reload"]]
	};

	window.clear(sf::Color::Cyan);
    window.draw(optionMenu_picture);
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
				"right",
				"reload"
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
				menu_choose_sound.play();
			}
			if (event.key.code == sf::Keyboard::Up) {
				keysMenu.selected--;
				if (keysMenu.selected < 0) {
					keysMenu.selected = keysMenu.items.size() - 1;
				}
				menu_choose_sound.play();
			}
			if (event.key.code == sf::Keyboard::Return) {
				waiting_for_input = true;
			}
		}
	}
}

