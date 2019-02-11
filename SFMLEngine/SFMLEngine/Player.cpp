#include "Player.h"
using namespace Engine;

void Engine::Player::isKeyPressed()
{
	if (Keyboard::isKeyPressed(Keyboard::C))
	{
		if (isShoot == false)
			isShoot = true;
	}
	if (Keyboard::isKeyPressed(Keyboard::W))
	{
		if (onGround)
		{
			isWalk = false;
			directionY = DirectionY::Up;
			speedY = energy * 400;
			onGround = false;
		}
	}
	else if (Keyboard::isKeyPressed(Keyboard::A))
	{
		isWalk = true;
		if (direction != Left) speedX -= speedX * 0.8;
		direction = DirectionX::Left;
		speedX = (speedX < maxSpeed) ? speedX += energy : speedX = maxSpeed;
	}
	else if (Keyboard::isKeyPressed(Keyboard::D))
	{
		isWalk = true;
		if (direction != Right) speedX -= speedX * 0.8;
		direction = DirectionX::Right;
		speedX = (speedX < maxSpeed) ? speedX += energy : speedX = maxSpeed;
	}
	else
	{
		isWalk = false;
	}

}

void Engine::Player::RotateToMouse(float speed, sf::RenderWindow & w)
{
	LastAngle = sprite.getRotation();

	sf::Vector2f posMouse = w.mapPixelToCoords(sf::Mouse::getPosition(w), w.getView());
	float a = posMouse.x - sprite.getPosition().x;
	float v = posMouse.y - sprite.getPosition().y;

	Radian = atan2f(v, a);
	CurrAngle = (atan2f(v, a)) * 180 / PI;
	if (CurrAngle > 180) CurrAngle -= 360;
	if (CurrAngle < -180) CurrAngle += 360;
	posMouse.x = CurrAngle - LastAngle;
	posMouse.y = 360 + posMouse.x;
	if (posMouse.y > 360) posMouse.y -= 720;
	if (abs(posMouse.x) > abs(posMouse.y)) posMouse.x = posMouse.y;

	if (CurrAngle != LastAngle)
	{
		if (abs(posMouse.x) < speed)
		{
			LastAngle = CurrAngle;
		}
		else
		{
			if (posMouse.x > 0)
				LastAngle += speed;
			else
				LastAngle -= speed;
		}
	}

	float lastradian = LastAngle * PI / 180;
}

void Engine::Player::update(float time)
{
	switch (direction)
	{
	case Left:  velocity.x = -speedX; velocity.y = 0;  break;
	case Right: velocity.x = speedX; velocity.y = 0;   break;
	}

	switch (directionY)
	{
	case Up:  velocity.y = -speedY;  break;
	}

	if (!onGround)
	{
		speedY -= energy;
	}
	else
		velocity.y = speedY = 0;
	if (!isWalk)
	{
		(speedX > 0) ? speedX -= friction : speedX = 0;
	}
	localRectangle = sprite.getTextureRect();
	sf::Vector2f pos = { position.x - originOffset.x * scale, position.y - originOffset.y * scale };
	globalRectangle = sf::FloatRect(pos.x, pos.y, pos.x + localRectangle.width * scale, pos.y + localRectangle.height * scale);
	debugRectangle = sf::FloatRect(pos.x + localRectangle.width * scale, pos.y, pos.x, pos.y + localRectangle.height * scale);
	position += velocity * time;
	checkClashes(time);
	sprite.setPosition(position);
	inventory.update();
	dp.draw("Player", true);
	dw_a.draw("Actor", false, true);
}

void Engine::Player::checkClashes(float time)
{
	collision::SAT sat;
	for (auto& i : obj)
	{
		auto playerRect = Rectangle::fromSfmlRect(sprite.getGlobalBounds());
		auto objectRect = Rectangle::fromSfmlRect(i.rect);
		if (i.name == "barrier")
		{
			if (sprite.getGlobalBounds().intersects(i.rect))
			{
				isCollision = true;
				auto offset = Rectangle::GetIntersectionDepth(playerRect, objectRect);
				isWalk = false;

				if (offset.y != 0)
				{
					onGround = true;
				}
				if (offset.x != 0)
					offset.x *= 0.2 * time;

				position = position + offset.GetSfmlVector();
			}
		}
	}
}

void Engine::Player::handleEvent(sf::Event & e)
{
	if (inv.getShowGui())
		inv.handleEvent(e);
}
