#include <SFML/Graphics.hpp>
#include <iostream>

#include "Game.hpp"
#include "StartMenu.hpp"

int main()
{

Game game;

	sf::RenderWindow window(sf::VideoMode(600,600), "MAIN MENU");

	StartMenu text(window.getSize().x, window.getSize().y);

	while (window.isOpen()){

        sf::Event event;

        while (window.pollEvent(event)){

            switch (event.type){

            case sf::Event::KeyReleased:

                switch (event.key.code){

                case sf::Keyboard::Up:

                    text.MoveUp();
                    break;

                case sf::Keyboard::Down:

                    text.MoveDown();
                    break;

                case sf::Keyboard::Return:

                    switch (text.GetPressedItem()){

                    case 0:

                        window.close();
                        game.run();
                        break;

                    case 1:

                        std::cout << "Ten tu piruleta!!" << std::endl;
                        break;

                    case 2:

                        window.close();
                        break;
                    }
                    break;

                }

            break;

            case sf::Event::Closed:

                window.close();

                break;
            }

        }

        window.clear();
        text.draw(window);
        window.display();

	}


    return 0;

}

