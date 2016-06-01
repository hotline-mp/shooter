#include "Game.hpp"
#include <iostream>
#include <vector>
#include "Menu.hpp"


void Game::optionsMenuLoop()
{
	sf::View view = window.getView();
	sf::Vector2f size = view.getSize();
	sf::Vector2f position = (size/2.f);
	view.setCenter(size/2.f);
	window.setView(view);

	char sfxbuf[50];
	sprintf(sfxbuf, "%d / 100", sfx_volume);
	char musbuf[50];
	sprintf(musbuf, "%d / 100", music_volume);
	optionsMenu.values = {
		"",
		musbuf,
		sfxbuf,
		""
	};
	/*
	keysMenu.items = {
		"Key Mappings",
		"Music Volume",
		"SFX Volume",
		"Exit"
	 */

    window.clear(sf::Color(0xCC, 0xFF, 0xEE));
	optionsMenu.draw(window);
}

void Game::changeVolume(int a) {
	int *n = nullptr;
	if (optionsMenu.selected == 1) {
		n = &music_volume;
	} else if (optionsMenu.selected == 2) {
		n = &sfx_volume;
	} else {
		return;
	}
	if (*n + a >= 0 && *n + a <= 100) {
		*n += a;
	}
	setVolumes();
}

void Game::optionsMenuHandleEvent(sf::Event &event)
{
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Return) {
			if (optionsMenu.selected == 0) {
				previous_game_state.push_back(OptionsMenu);
				next_game_state = KeysMenu;
				std::cout << "keysmenu" << std::endl;
			} else if (optionsMenu.selected == 3) {
				next_game_state = previous_game_state.back();
				previous_game_state.pop_back();
			}
		} else if (event.key.code == sf::Keyboard::Down) {
			optionsMenu.selected++;
			if (optionsMenu.selected >= (int)optionsMenu.items.size()) {
				optionsMenu.selected = 0;
			}
			menu_choose_sound.play();
		} else if (event.key.code == sf::Keyboard::Up) {
			optionsMenu.selected--;
			if (optionsMenu.selected < 0) {
				optionsMenu.selected = optionsMenu.items.size() - 1;
			}
			menu_choose_sound.play();
		} else if (event.key.code == sf::Keyboard::Escape) {
			next_game_state = previous_game_state.back();
			previous_game_state.pop_back();
		} else if (event.key.code == sf::Keyboard::Right) {
			changeVolume(10);
		} else if (event.key.code == sf::Keyboard::Left) {
			changeVolume(-10);
		}
	}
}

