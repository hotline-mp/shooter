#include "Player.hpp"

Player::Player(sf::RenderWindow &window, sf::Vector2f &camera) :
	Entity(window, camera)
{
	shape.setFillColor(sf::Color::Green);
	if(!texture.loadFromFile("player.png"));
	sprite.setTexture(texture);
}

void Player::draw() {
    sprite.setPosition(position + sf::Vector2f(30,30) + camera);
	sf::Vector2f curPos = sprite.getPosition();
    sf::Vector2i position = sf::Mouse::getPosition(window);



    const float PI = 3.14159265;

    float dx = curPos.x - position.x;
    float dy = curPos.y - position.y;

    float rotation = (atan2(dy, dx)) * 180 / PI;


    sprite.setOrigin(30,30);
	sprite.setRotation(rotation);
	window.draw(sprite);
}

