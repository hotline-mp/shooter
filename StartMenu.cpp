#include "StartMenu.hpp"


StartMenu::StartMenu(float width, float height){

if (!font.loadFromFile("sans.ttf")){

    // handle error
}

menu[0].setFont(font);
menu[0].setColor(sf::Color::Red);
menu[0].setString("Play");
menu[0].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 1))

menu[1].setFont(font);
menu[1].setColor(sf::Color::White);
menu[1].setString("Options");
menu[1].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 2))

menu[2].setFont(font);
menu[2].setColor(sf::Color::White);
menu[2].setString("Exit");
menu[2].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 3))

SelectedItemIndex = 0;

}



void StartMenu::draw(sf::RenderWindow &window){

for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++){

    window.draw(menu[i]);

}

}

void StartMenu::MoveUp(){

if (SelectedItemIndex - 1 >= 0){

    menu[SelectedItemIndex].setColor(sf::Color::White);
    SelectedItemIndex--;
    menu[SelectedItemIndex].setColor(sf::Color::Red);

}
}

void StartMenu::MoveDown(){

if (SelectedItemIndex + 1 < MAX_NUMBER_OF_ITEMS){

    menu[SelectedItemIndex].setColor(sf::Color::White);
    SelectedItemIndex++;
    menu[SelectedItemIndex].setColor(sf::Color::Red);

}
}

int StartMenu::GetPressedItem(){

return SelectedItemIndex;

}









