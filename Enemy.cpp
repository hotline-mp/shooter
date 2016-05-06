#include "Enemy.hpp"
#include "Player.hpp"
#include "vector.hpp"

Enemy::Enemy(sf::Texture *texture, sf::Clock *clock, sf::RenderWindow *window,
		sf::Vector2f *camera) :
	Entity(clock, window, camera, 0.0003f, 30.f)
{
	//if (!texture.loadFromFile("enemy1.png")) {
	//if (!texture.loadFromFile("player.png")) {
	//	exit(1);
	//}
	//this->texture = texture;
	sprite.setTexture(*texture);
	frame = 0;
	//frames = {sf::IntRect(0,0,60,60), sf::IntRect(60,0,71,60),sf::IntRect(0,0,60,60),sf::IntRect(131,0,71,60)};
	frames = {sf::IntRect(0,0,60,60)};
}

void Enemy::nextFrame() {
    if ((unsigned)frame == frames.size() - 1) {
        frame = 0;
    } else {
        frame++;
    }
}

void Enemy::draw() {
    sprite.setPosition(position + sf::Vector2f(30,30) + *camera);
    sprite.setTextureRect(frames[frame]);

    sprite.setOrigin(30,30);
	window->draw(sprite);
}

void Enemy::update(Player player, std::vector< std::vector<sf::Vector2f> > map) {
	moving = vecUnit(player.position - position);

	// common
	sf::Time time_now = clock->getElapsedTime();
	if (lastUpdated.asMicroseconds() == 0) {
		// El primer frame del joc ningú es mou
		lastUpdated = time_now;
		return;
	}
	long long micros = time_now.asMicroseconds() - lastUpdated.asMicroseconds();
	const float vel = 0.00005; // pixels / ms
	//position += moving * vel * micros;
	target_movement = moving * float(vel * micros);
	lastUpdated = clock->getElapsedTime();

    if (target_movement == sf::Vector2f(0, 0)) {
        frame = 0;
    } else if ((time_now - lastAnimFrame).asMilliseconds() > 300) {
        nextFrame();
        lastAnimFrame = time_now;
	}
}

