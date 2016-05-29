#include <math.h>
#include <algorithm>
#include "Player.hpp"
#include "vector.hpp"

Player::Player(sf::Clock *clock, sf::RenderWindow *window) :
	Entity(clock, window, 0.0003f, 20.f)
{
	reset();

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
	frames = {sf::IntRect(0,0,37,75), sf::IntRect(212,0,37,75), sf::IntRect(170,0,37,75),
        sf::IntRect(128,0,37,75), sf::IntRect(86,0,37,75), sf::IntRect(44,0,37,75),
        sf::IntRect(86,0,37,75), sf::IntRect(128,0,37,75), sf::IntRect(170,0,37,75),
        sf::IntRect(212,0,37,75), sf::IntRect(0,0,37,75), sf::IntRect(254,0,37,75),
        sf::IntRect(296,0,37,75), sf::IntRect(338,0,37,75), sf::IntRect(380,0,37,75),
        sf::IntRect(422,0,37,75), sf::IntRect(380,0,37,75), sf::IntRect(338,0,37,75),
        sf::IntRect(296,0,37,75), sf::IntRect(254,0,37,75)};
}

void Player::reset() {
	ammo = mag_size;
	extra_ammo = mag_size*2;
	reloading = false;
	hp = 100;
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
	//sf::Vector2f curPos = torso.getPosition();
    //sf::Vector2i position = sf::Mouse::getPosition(*window);

	sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
	sf::Vector2f mouse_pos = window->mapPixelToCoords(pixelPos);

    piernas.setTextureRect(frames[frame]);

    const float PI = 3.14159265;

    float dx = position.x - mouse_pos.x;
    float dy = position.y - mouse_pos.y;

    float rotation = (atan2(dy, dx)) * 180 / PI;
    float feet_rotation = (atan2(moving.y, moving.x)) * 180 / PI;
	// Si no ens estem movent, els peus avall, però si no, fa falta sumar 90 graus
	if (moving != sf::Vector2f(0, 0)) {
		feet_rotation += 90;
	}

	piernas.setRotation(feet_rotation);
	torso.setRotation(rotation);
    piernas.setPosition(this->position);
    torso.setPosition(this->position);
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

	if (reloading && reload_timer < time_now) {
		reloading = false;
		int diff = std::min(mag_size, extra_ammo) - ammo;
		ammo += diff;
		extra_ammo -= diff;
	}
}

void Player::hit(sf::Vector2f direction) {
	moving = vecUnit(direction)*5.f;
}

