#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Menu {
	private:
		sf::Font font;
		sf::Text text;

	public:
		Menu();
		int selected = 0;
		std::vector<std::string> items;
		std::vector<std::string> values;
		std::string title = "";
		void draw(sf::RenderWindow &window);
};

#endif /* !MENU_H */
