#include <iostream>
#include <SFML/System/Vector2.hpp>
#include "Bullet.hpp"
#include "vector.hpp"

Bullet::Bullet(sf::Clock *clock, sf::RenderWindow *window, sf::Vector2f vel) :
	//Particle(clock, window, 0.001f /*vel*/, 5 /*radius*/, 0 /*accel*/)
	Particle(clock, window, 5 /*radius*/, vel, 0)
{
	shape.setFillColor(sf::Color::Black);
	shape.setOrigin(radius, radius);
}

