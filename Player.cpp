#include <math.h>
#include "Player.hpp"
#include "vector.hpp"

Player::Player(sf::Clock *clock, sf::RenderWindow *window, sf::Vector2f *camera) :
	Entity(clock, window, camera, 0.0003f, 20.f)
{
    if(!body.loadFromFile("playertorso.png")) {
		exit(1);
    }
    torso.setTexture(body);
	torso.setOrigin(49,22);
	if (!legs.loadFromFile("playerpiernas.png")) {
		exit(1);
	}
	piernas.setTexture(legs);
    piernas.setOrigin(18,44);
	frame = 0;
	frames = {sf::IntRect(0,0,37,75), sf::IntRect(185,0,37,75), sf::IntRect(148,0,37,75),
        sf::IntRect(111,0,37,75), sf::IntRect(74,0,37,75), sf::IntRect(37,0,37,75),
        sf::IntRect(74,0,37,75), sf::IntRect(111,0,37,75), sf::IntRect(148,0,37,75),
        sf::IntRect(185,0,37,75), sf::IntRect(0,0,37,75), sf::IntRect(222,0,37,75),
        sf::IntRect(259,0,37,75), sf::IntRect(296,0,37,75), sf::IntRect(333,0,37,75),
        sf::IntRect(370,0,37,75), sf::IntRect(333,0,37,75), sf::IntRect(296,0,37,75),
        sf::IntRect(259,0,37,75), sf::IntRect(222,0,37,75)};
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
	sf::Vector2f curPos = torso.getPosition();
    sf::Vector2i position = sf::Mouse::getPosition(*window);
    piernas.setTextureRect(frames[frame]);

    const float PI = 3.14159265;

    float dx = curPos.x - position.x;
    float dy = curPos.y - position.y;

    float rotation = (atan2(dy, dx)) * 180 / PI;
    float feet_rotation = (atan2(moving.y, moving.x)) * 180 / PI;
	// Si no ens estem movent, els peus avall, però si no, fa falta sumar 90 graus
	if (moving != sf::Vector2f(0, 0)) {
		feet_rotation += 90;
	}

	piernas.setRotation(feet_rotation);
	torso.setRotation(rotation);
    piernas.setPosition(this->position + *camera);
    torso.setPosition(this->position + *camera);
	window->draw(piernas);
	window->draw(torso);
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
    } else if ((time_now - lastAnimFrame).asMilliseconds() > 50) {
        nextFrame();
        lastAnimFrame = time_now;
	}
}

void Player::hit(sf::Vector2f direction) {
	moving = vecUnit(direction)*5.f;
}

