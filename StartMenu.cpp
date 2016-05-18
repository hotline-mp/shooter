#include "StartMenu.hpp"


StartMenu::StartMenu(float width, float height){

if (!font.loadFromFile("sans.ttf")){

    // handle error
}

text[0].setFont(font);
text[0].setColor(sf::Color::Red);
text[0].setString("Play");
text[0].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 1));

text[1].setFont(font);
text[1].setColor(sf::Color::White);
text[1].setString("Options");
text[1].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 2));

text[2].setFont(font);
text[2].setColor(sf::Color::White);
text[2].setString("Exit");
text[2].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 3));

SelectedItemIndex = 0;

}



void StartMenu::draw(sf::RenderWindow &window){

for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++){

    window.draw(text[i]);

}

}

void StartMenu::MoveUp(){

if (SelectedItemIndex - 1 >= 0){

    text[SelectedItemIndex].setColor(sf::Color::White);
    SelectedItemIndex--;
    text[SelectedItemIndex].setColor(sf::Color::Red);

}
}

void StartMenu::MoveDown(){

if (SelectedItemIndex + 1 < MAX_NUMBER_OF_ITEMS){

    text[SelectedItemIndex].setColor(sf::Color::White);
    SelectedItemIndex++;
    text[SelectedItemIndex].setColor(sf::Color::Red);

}
}

int StartMenu::GetPressedItem(){

return SelectedItemIndex;

}









