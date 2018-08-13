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
	float rotateSpeed = 0.7;
	bool rot = false;
	LastAngle = sprite.getRotation();

	Vector2i pixelPos = Mouse::getPosition(window);
	Vector2f posMouse = window.mapPixelToCoords(pixelPos, window.getView());
	float a = posMouse.x - sprite.getPosition().x;
	float v = posMouse.y - sprite.getPosition().y;

	Radian = atan2f(v, a);
	float rotation = (atan2f(v, a)) * 180 / 3.14159265;
	CurrAngle = rotation; //?

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
		if (globalRectangle.getSfmlRect_f().intersects(obj[i].rect)) // Столкновение персоннажа с объектами карты
		{
			if (obj[i].name == "barrier")
			{
				auto z = Rectangle::GetIntersectionDepth(globalRectangle, Rectangle(obj[i].rect.left, obj[i].rect.top, obj[i].rect.width + obj[i].rect.left, obj[i].rect.height + obj[i].rect.top));
				std::cout << z << std::endl;
			}
		}
	}
}

void Engine::Actor::update(float time)
{
	switch (direction)
	{
	case Up:   velocity.y = -speed; velocity.x = 0;  break;
	case Down: velocity.y = speed; velocity.x = 0;  break;
	case Left: velocity.x = -speed; velocity.y = 0;  break;
	case Right:velocity.x = speed; velocity.y = 0;  break;
	}
	globalRectangle = Rectangle(position.x - 90, position.y - 120, localRectangle.w, localRectangle.h);
	RotateToMouse(0.7, *window);
	position += velocity * time;
	checkClashes(position);
	sprite.setPosition(position.GetSfmlVector());
}

void Engine::Actor::getDamage(float dmg)
{
	lives -= dmg * ((100 - armor) / 100);
}
