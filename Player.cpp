#include <math.h>
#include "Player.hpp"
#include "vector.hpp"

Player::Player(sf::Clock *clock, sf::RenderWindow *window, sf::Vector2f *camera) :
	Entity(clock, window, camera, 0.0003f, 20.f)
{
    if(!body.loadFromFile("playertorso.png")) {
		exit(1);
    }
    jugador.setTexture(body);
	jugador.setOrigin(40,30);
	if (!texture.loadFromFile("playerpiernas.png")) {
		exit(1);
	}
	sprite.setTexture(texture);
    sprite.setOrigin(40,30);
	frame = 0;
	frames = {sf::IntRect(0,0,60,60), sf::IntRect(60,0,71,60),
		sf::IntRect(0,0,60,60), sf::IntRect(131,0,71,60)};
}

void Player::nextFrame() {
    if ((unsigned)frame == frames.size() - 1) {
        frame = 0;
    } else {
        frame++;
    }
}

void Player::draw() {
	if (!visible) {
		return;
	}
	sf::Vector2f curPos = jugador.getPosition();
    sf::Vector2i position = sf::Mouse::getPosition(*window);
    sprite.setTextureRect(frames[frame]);
    jugador.setTextureRect(frames[frame]);

    const float PI = 3.14159265;

    float dx = curPos.x - position.x;
    float dy = curPos.y - position.y;

    float rotation = (atan2(dy, dx)) * 180 / PI;
    float feet_rotation = (atan2(moving.y, moving.x)) * 180 / PI;
	// Si no ens estem movent, els peus avall, però si no, fa falta sumar 90 graus
	if (moving != sf::Vector2f(0, 0)) {
		feet_rotation += 90;
	}

	sprite.setRotation(feet_rotation);
	jugador.setRotation(rotation);
    sprite.setPosition(this->position + *camera);
    jugador.setPosition(this->position + *camera);
	window->draw(sprite);
	window->draw(jugador);
}

void Player::update() {
	sf::Time time_now = clock->getElapsedTime();
	if (lastUpdated.asMicroseconds() == 0) {
		// El primer frame del joc ningú es mou
		lastUpdated = time_now;
		return;
	}
	long long micros = time_now.asMicroseconds() - lastUpdated.asMicroseconds();
	const float vel = 0.0003; // pixels / ms
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

void Player::hit(sf::Vector2f direction) {
	moving = vecUnit(direction)*5.f;
}

