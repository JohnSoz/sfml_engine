#include "Object.h"
using namespace Engine;
using namespace sf;
Engine::Entity::Entity(Vector2D POSITION, std::string NAME) : position(POSITION), name(NAME)
{
}

Engine::Entity::Entity(sf::Image & IMAGE, Vector2D POSITION, std::string NAME) : position(POSITION), name(NAME)
{
	texture.loadFromImage(IMAGE);
	texture.setSmooth(true);
	sprite.setTexture(texture);
	sprite.setPosition(position.GetSfmlVector());
}

Engine::Entity::Entity(sf::Image & i, sf::IntRect r, Vector2D pos, std::string name)
{
}

Engine::Entity::~Entity()
{
}

void Engine::Actor::RotateToMouse(float speed, sf::RenderWindow& window)
{
	float rotateSpeed = speed;
	bool rot = false;
	LastAngle = sprite.getRotation();

	Vector2i pixelPos = Mouse::getPosition(window);
	Vector2f posMouse = window.mapPixelToCoords(pixelPos, window.getView());
	float a = posMouse.x - sprite.getPosition().x;
	float v = posMouse.y - sprite.getPosition().y;

	Radian = atan2f(v, a);
	float rotation = (atan2f(v, a)) * 180 / 3.14159265;
	CurrAngle = rotation; //?

	auto x = globalRectangle.x;
	auto y = globalRectangle.y;
	auto w = globalRectangle.w;
	auto h = globalRectangle.h;

	float centerX = sprite.getPosition().x;
	float centerY = sprite.getPosition().y;

	auto newX = centerX + (x - centerX) * cos(Radian) - (y - centerY) * sin(Radian);//x,y
	auto newW = centerX + (w - centerX) * cos(Radian) - (h - centerY) * sin(Radian);//w,h
	auto newY = centerY + (y - centerY) * cos(Radian) + (x - centerX) * sin(Radian);//y,x
	auto newH = centerY + (h - centerY) * cos(Radian) + (w - centerX) * sin(Radian);//h,w

	auto newX2 = centerX + (x - centerX) * cos(Radian) - (h - centerY) * sin(Radian); //x,h
	auto newY2 = centerY + (h - centerY) * cos(Radian) + (x - centerX) * sin(Radian); //h,x
	auto newW2 = centerX + (w - centerX) * cos(Radian) - (y - centerY) * sin(Radian); //w,y
	auto newY3 = centerY + (y - centerY) * cos(Radian) + (w - centerX) * sin(Radian); //y,w

	//auto newW2 = centerX + ((w - 30) - centerX) * cos(Radian) - ((y + 15) - centerY) * sin(Radian); //w,y
	//auto newY3 = centerY + ((y + 15) - centerY) * cos(Radian) + ((w - 30) - centerX) * sin(Radian); //y,w

	debugRectangle.x = newX2;//x2
	debugRectangle.y = newY2;//y2
	debugRectangle.w = newW2;//w2
	debugRectangle.h = newY3;//h2

	globalRectangle.x = newX;
	globalRectangle.y = newY;
	globalRectangle.w = newW;
	globalRectangle.h = newH;

	if (CurrAngle > 180) CurrAngle -= 360;
	if (CurrAngle < -180) CurrAngle += 360;
	posMouse.x = CurrAngle - LastAngle;
	posMouse.y = 360 + posMouse.x;
	if (posMouse.y > 360) posMouse.y -= 720;
	if (abs(posMouse.x) > abs(posMouse.y)) posMouse.x = posMouse.y;

	if (CurrAngle == LastAngle)
		return;

	if (abs(posMouse.x) < rotateSpeed)
	{
		LastAngle = CurrAngle;
	}
	else
	{
		if (posMouse.x > 0)
			LastAngle += rotateSpeed;
		else
			LastAngle -= rotateSpeed;
	}
	if (window.hasFocus())
	{
		sprite.setRotation(LastAngle);
	}
}

std::string Engine::Actor::debugInfo()
{
	std::string s;
	s += "\n";
	s += " isWalk = " + std::to_string(isWalk) + "\n";
	s += " isCollision = " + std::to_string(isCollision) + "\n";
	s += " Direction = " + std::to_string(direction) + "\n";
	s += " velX = " + std::to_string(velocity.x) + " velY = " + std::to_string(velocity.y) + "\n";
	s += " speed = " + std::to_string(speed) + "\n";
	s += " Angle = " + std::to_string(CurrAngle) + "\n";
	return s;
}

void Engine::Actor::handleEvent(sf::Event & e)
{
	if (Keyboard::isKeyPressed(Keyboard::W))
	{
		isWalk = true;
		direction = Direction::Up;
		speed = (speed < maxSpeed) ? speed += energy : speed = maxSpeed;
	}
	else if (Keyboard::isKeyPressed(Keyboard::S))
	{
		isWalk = true;
		direction = Direction::Down;
		speed = (speed < maxSpeed) ? speed += energy : speed = maxSpeed;
	}
	else if (Keyboard::isKeyPressed(Keyboard::A))
	{
		isWalk = true;
		direction = Direction::Left;
		speed = (speed < maxSpeed) ? speed += energy : speed = maxSpeed;
	}
	else if (Keyboard::isKeyPressed(Keyboard::D))
	{
		isWalk += true;
		direction = Direction::Right;
		speed = (speed < maxSpeed) ? speed += energy : speed = maxSpeed;
	}
	else
	{
		isWalk = false;
		(speed > 0) ? speed -= friction : speed = 0;
	}
}

void Engine::Actor::checkClashes(Vector2D pos)
{
	for (int i = 0; i < obj.size(); i++)
	{
		sf::FloatRect Intersection;
		sf::FloatRect objGlobalRect = { obj[i].rect.left, obj[i].rect.top, obj[i].rect.width, obj[i].rect.height };
		auto zzxc = sprite.getGlobalBounds();
		if (sprite.getGlobalBounds().intersects(objGlobalRect, Intersection))
		{
			if (obj[i].name == "barrier")
			{
				float offset = (abs(Intersection.width) > abs(Intersection.height)) ? Intersection.height : Intersection.width;
				isCollision = true;
				std::cout << "W = " << Intersection.width << " H = " << Intersection.height << " Top = " << Intersection.top << " Left = " << Intersection.left << std::endl;
			}
		}
		else
			isCollision = false;
	}
}

void Engine::Actor::update(float time)
{
	switch (direction)
	{
	case Up:   velocity.y = -speed; velocity.x = 0;  break;
	case Down: velocity.y = speed; velocity.x = 0;  break;
	case Left: velocity.x = -speed; velocity.y = 0;  break;
	case Right: velocity.x = speed; velocity.y = 0;  break;
	}
	globalRectangle = Rectangle(position.x - originOffset.x, position.y - originOffset.y, position.x - originOffset.x + localRectangle.w, position.y - originOffset.y + localRectangle.h);
	RotateToMouse(0.2 * time, *window);
	position += velocity * time;
	checkClashes(position);
	sprite.setPosition(position.GetSfmlVector());
}

void Engine::Actor::getDamage(float dmg)
{
	lives -= dmg * ((100 - armor) / 100);
}
