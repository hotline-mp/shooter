#ifndef BALA_H
#define BALA_H

#include <SFML/Graphics.hpp>
#include "Entity.hpp"



class Bala : public Entity{

public:
    const float bala_r=5;
    void draw();
    Bala(sf::Clock &clock, sf::RenderWindow &window, sf::Vector2f &camera);
private:
    sf::CircleShape shape{bala_r};

};



#endif /* !BALA_H */
