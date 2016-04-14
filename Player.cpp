#include "Player.hpp"

Player::Player(sf::Clock *clock, sf::RenderWindow *window, sf::Vector2f *camera) :
	Entity(clock, window, camera, 0.0003f, 30.f),
	shape(30.f)
{
    if(!body.loadFromFile("playertorso.png"))
    {
    }
    jugador.setTexture(body);
	if (!texture.loadFromFile("playerpiernas.png")) {
		exit(1);
	}
	sprite.setTexture(texture);
	frame = 0;
	frames = {sf::IntRect(0,0,60,60), sf::IntRect(60,0,71,60),sf::IntRect(0,0,60,60),sf::IntRect(131,0,71,60)};
}

void Player::nextFrame() {
    if ((unsigned)frame == frames.size() - 1) {
        frame = 0;
    } else {
        frame++;
    }
}

void Player::draw() {
    sprite.setPosition(position + sf::Vector2f(30,30) + *camera);
    jugador.setPosition(position + sf::Vector2f(30,30) + *camera);
    int x;
    int y;
    int direction;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		x = -1;
		direction=0;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		x = 1;
		direction=180;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		y = -1;
		direction=90;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		y = 1;
		direction=270;
	}
	if(x==-1 && y==-1)
    {
        direction=45;
    }
    else if(x==1 && y==-1)
    {
        direction=135;
    }
    else if(x==1 && y==1)
    {
        direction=225;
    }
    else if(x==-1 && y==1)
    {
        direction=315;
    }
	sf::Vector2f curPos = jugador.getPosition();
    sf::Vector2i position = sf::Mouse::getPosition(*window);
    sprite.setTextureRect(frames[frame]);

    const float PI = 3.14159265;

    float dx = curPos.x - position.x;
    float dy = curPos.y - position.y;

    float rotation = (atan2(dy, dx)) * 180 / PI;


    sprite.setOrigin(50,30);
	sprite.setRotation(direction);
	jugador.setOrigin(50,30);
	jugador.setRotation(rotation);
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

