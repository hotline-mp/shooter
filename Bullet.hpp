#ifndef BALA_H
#define BALA_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include "Particle.hpp"
#include "Enemy.hpp"

class Bullet : public Particle {
	public:
		Bullet(sf::Clock *clock, sf::RenderWindow *window, sf::Vector2f vel);
		std::vector<Enemy*> enemies_hit;
};

#endif /* !BALA_H */
