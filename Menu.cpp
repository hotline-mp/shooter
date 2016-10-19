#include "Menu.hpp"
#include <iostream>

Menu::Menu() {
	std::cout << "loading font" << std::endl;
	if (!font.loadFromFile("sans.ttf")) {
		std::cerr << "Could not load font" << std::endl;
		exit(2);
	}
	text.setFont(font);
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::Black);
}

void Menu::draw(sf::RenderWindow &window) {
	int x = 60;
	int y = 80;
	text.setPosition(x, y);
	text.setString(title);
	text.setStyle(sf::Text::Underlined);
	window.draw(text);
	y += 40;
	for (unsigned int i=0; i<items.size(); i++) {
		text.setStyle(0);
		text.setPosition(x, y);
		text.setString(items[i]);
		if ((unsigned)selected == i) {
			text.setStyle(sf::Text::Bold);
		}
		window.draw(text);
		if (values.size() != 0) {
			text.setPosition(x + 200, y);
			text.setString(values[i]);
			window.draw(text);
		}
		y += 30;
	}
}

