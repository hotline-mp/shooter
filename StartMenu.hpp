#pragma once
#include "SFML/Graphics.hpp"

#define MAX_NUMBER_OF_ITEMS 3

class StartMenu{

public:

    StartMenu(float width, float height);


    void draw(sf::RenderWindow &window);
    void MoveUp();
    void MoveDown();
    int GetPressedItem();

private:

    int SelectedItemIndex;
    sf::Font font;
    sf::Text text[MAX_NUMBER_OF_ITEMS];

};

