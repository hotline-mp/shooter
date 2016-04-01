#include "Player.hpp"

Player::Player(sf::RenderWindow &window, sf::Vector2f &camera) :
	Entity(window, camera)
{
	if(!texture.loadFromFile("player.png"));
	sprite.setTexture(texture);
	frame = 0;
	frames = {sf::IntRect(0,0,60,60), sf::IntRect(60,0,71,60),sf::IntRect(0,0,60,60),sf::IntRect(131,0,71,60)};
}

void Player::nextFrame() {
    if (frame == frames.size() - 1) {
        frame = 0;
    } else {
        frame++;
    }
}

void Player::draw() {
    sprite.setPosition(position + sf::Vector2f(30,30) + camera);
	sf::Vector2f curPos = sprite.getPosition();
    sf::Vector2i position = sf::Mouse::getPosition(window);
    sprite.setTextureRect(frames[frame]);

    const float PI = 3.14159265;

    float dx = curPos.x - position.x;
    float dy = curPos.y - position.y;

    float rotation = (atan2(dy, dx)) * 180 / PI;


    sprite.setOrigin(50,30);
	sprite.setRotation(rotation);
	window.draw(sprite);
}

void Player::update() {
	sf::Time time_now = clock.getElapsedTime();
	if (lastUpdated.asMicroseconds() == 0) {
		// El primer frame del joc ningú es mou
		lastUpdated = time_now;
		return;
	}
	long long micros = time_now.asMicroseconds() - lastUpdated.asMicroseconds();
	const float vel = 0.0003; // pixels / ms
	//position += moving * vel * micros;
	target_movement = moving * float(vel * micros);
	lastUpdated = clock.getElapsedTime();

    if (target_movement == sf::Vector2f(0, 0)) {
        frame = 0;
    } else if ((time_now - lastAnimFrame).asMilliseconds() > 300) {
        nextFrame();
        lastAnimFrame = time_now;
	}
}

