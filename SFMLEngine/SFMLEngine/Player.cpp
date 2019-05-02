#include <entityx/entityx.h>
#include "Player.h"
using namespace Engine;

void Player::isKeyPressed()
{
	if (Keyboard::isKeyPressed(Keyboard::W) && onGround)
	{
		isWalk = false;
		directionY = Up;
		onGround = false;
		velocity.y = -(energy * 120.f);
	}
	if (Keyboard::isKeyPressed(Keyboard::A))
	{
		isWalk = true;
		if (direction != Left)
			velocity.x -= velocity.x * 0.85f;
		direction = Left;
		velocity.x = (abs(velocity.x) < maxSpeed) ? velocity.x -= energy : velocity.x = -maxSpeed;
	}
	else if (Keyboard::isKeyPressed(Keyboard::D))
	{
		isWalk = true;
		if (direction != Right)
			velocity.x -= velocity.x * 0.85f;
		direction = Right;
		velocity.x = (velocity.x < maxSpeed) ? velocity.x += energy : velocity.x = maxSpeed;
	}
	else
	{
		isWalk = false;
	}
	if (Keyboard::isKeyPressed(Keyboard::I))
		if (Pressclock.getElapsedTime().asMilliseconds() > 500)
		{
			EventManager::eventManager.emit<Events::Event_Inventory_UI>();
			Pressclock.restart();
		}
}

void Player::RotateToMouse(float speed, RenderWindow& w)
{
	LastAngle = sprite.getRotation();

	auto posMouse = w.mapPixelToCoords(Mouse::getPosition(w), w.getView());
	const float a = posMouse.x - sprite.getPosition().x;
	const float v = posMouse.y - sprite.getPosition().y;

	Radian = atan2f(v, a);
	CurrAngle = (double)(atan2f(v, a)) * 180.f / PI;

	if (CurrAngle > 180)
		CurrAngle -= 360;

	if (CurrAngle < -180)
		CurrAngle += 360;

	posMouse.x = CurrAngle - LastAngle;
	posMouse.y = 360 + posMouse.x;

	if (posMouse.y > 360)
		posMouse.y -= 720;

	if (abs(posMouse.x) > abs(posMouse.y))
		posMouse.x = posMouse.y;

	if (CurrAngle != LastAngle)
	{
		if (abs(posMouse.x) < speed)
			LastAngle = CurrAngle;
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

void Player::update(float time)
{
	switch (direction)
	{
	case Left:
		//velocity.x = -speedX;
		break;
	case Right:
		//velocity.x = speedX;
		break;
	}

	switch (directionY)
	{
	case Up:
		//velocity.y = -speedY;
		break;
	default:;
	}
	//if(velocity.y < 0)
	//velocity.y += grav;

	if (velocity.y < 0)
		isJump = true;

	if (!onGround)
		velocity.y += energy;
	else
		velocity.y = 0;

	if (!isWalk)
		(velocity.x > 0) ? velocity.x -= friction : velocity.x = 0;

	position += velocity * time;
	checkClashes(time);

	sprite.setPosition(position);
	if (isWalk)
	{
		animManager.SetCurrAnimation(animManager.GetAnimationByName("Walk"));
		updateSprite();
	}
	else if (isJump)
	{
		animManager.SetCurrAnimation(animManager.GetAnimationByName("Jump"));
		updateSprite();
		isJump = false;
	}
	else if (!isWalk)
	{
		animManager.SetCurrAnimation(animManager.GetAnimationByName("Idle"));
		updateSprite();
	}
	sprite.setTextureRect(animManager.AnimUpdate(time));

	inventoryAction();
	inventory.update();

	localRectangle = sprite.getTextureRect();
	const sf::Vector2f pos = { position.x - originOffset.x * scale, position.y - originOffset.y * scale };

	globalRectangle = sf::FloatRect(pos.x, pos.y,
		pos.x + localRectangle.width * scale,
		pos.y + localRectangle.height * scale);

	debugRectangle = sf::FloatRect(pos.x + localRectangle.width * scale,
		pos.y, pos.x,
		pos.y + localRectangle.height * scale);
	debug::debugDraw<Player, Player, Object, Actor>(this, "Debug For Class Player");
}
#include <future> 
void Player::checkClashes(const float& time)
{
	isCollision = false;
	ray = { sprite.getPosition().x - getRect().width + sprite.getOrigin().x / 2,sprite.getPosition().y + sprite.getOrigin().y / 2};
	ray2 = { sprite.getPosition().x + getRect().width - sprite.getOrigin().x / 2,sprite.getPosition().y + sprite.getOrigin().y / 2};

	sf::Vector2f pos1 = { sprite.getPosition().x - getRect().width + sprite.getOrigin().x / 2, sprite.getPosition().y };
	sf::Vector2f pos2 = { sprite.getPosition().x + getRect().width - sprite.getOrigin().x / 2, sprite.getPosition().y };
	float dist = raycastLevelObject(pos1, { 0, sprite.getOrigin().y / 2 }, obj, "barrier");
	float dist2 = raycastLevelObject(pos2, { 0, sprite.getOrigin().y / 2 }, obj, "barrier");
	//std::cout << "dist = " << (float)(dist) << "   ___   " << "dist2 = " << (float)(dist2) << endl;

	if (dist < 1 && dist2 < 1)
		onGround = false;
	else
		onGround = true;
	for (const auto& i : obj)
	{
		auto playerRect = Rectangle::fromSfmlRect(sprite.getGlobalBounds());
		auto objectRect = Rectangle::fromSfmlRect(i.rect);
		if (i.name == "barrier")
		{
			if (sprite.getGlobalBounds().intersects(i.rect))
			{
				isCollision = true;

				auto copyOffset = Rectangle::GetIntersectionDepth(playerRect, objectRect).GetSfmlVector();

				if (abs(copyOffset.x) > abs(copyOffset.y))
					copyOffset.x = 0;
				else
					copyOffset.y = 0;

				if (copyOffset.y > 0)
				{
					velocity.y = 0;
					onGround = false;
				}

				position = position + copyOffset;
			}
		}
	}
}

void Engine::Player::handleEvent(sf::Event& e)
{
	if (inv.IsEnable())
		inv.handleEvent(e);
	if (e.type == sf::Event::KeyPressed)
	{
		if (e.key.code == sf::Keyboard::C)
		{
			isInvAction = !isInvAction;
		}
	}
}

#include "serializer.h"

Player::~Player()
{
	save<Player, Actor, Object>(*this);
}
