#include <entityx/entityx.h>
#include "Player.h"
using namespace Engine;

void Player::isKeyPressed()
{
	if (Keyboard::isKeyPressed(Keyboard::C))
	{
		if (!isInvAction)
			isInvAction = true;
	}
	if (Keyboard::isKeyPressed(Keyboard::W) && onGround)
	{
		isWalk = false;
		directionY = Up;
		speedY = energy * 110.f;
		onGround = false;
	}
	if (Keyboard::isKeyPressed(Keyboard::A))
	{
		isWalk = true;
		if (direction != Left)
			speedX -= speedX * 0.85f;
		direction = Left;
		speedX = (speedX < maxSpeed) ? speedX += energy : speedX = maxSpeed;
	}
	else if (Keyboard::isKeyPressed(Keyboard::D))
	{
		isWalk = true;
		if (direction != Right)
			speedX -= speedX * 0.85f;
		direction = Right;
		speedX = (speedX < maxSpeed) ? speedX += energy : speedX = maxSpeed;
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
	const auto a = posMouse.x - sprite.getPosition().x;
	const auto v = posMouse.y - sprite.getPosition().y;

	Radian = atan2f(v, a);
	CurrAngle = (atan2f(v, a)) * 180 / PI;

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
		velocity.x = -speedX;
		break;
	case Right:
		velocity.x = speedX;
		break;
	}

	switch (directionY)
	{
	case Up:
		velocity.y = -speedY;
		break;
	default:;
	}

	if (!onGround)
		speedY -= energy;
	else
		speedY = 0;

	if (!isWalk)
		(speedX > 0) ? speedX -= friction : speedX = 0;

	localRectangle = sprite.getTextureRect();
	const sf::Vector2f pos = { position.x - originOffset.x * scale, position.y - originOffset.y * scale };

	globalRectangle = sf::FloatRect(pos.x, pos.y,
		pos.x + localRectangle.width * scale,
		pos.y + localRectangle.height * scale);

	debugRectangle = sf::FloatRect(pos.x + localRectangle.width * scale,
		pos.y, pos.x,
		pos.y + localRectangle.height * scale);

	position += velocity * time;
	checkClashes(time);
	sprite.setPosition(position);
	inventory.update();
	inventoryAction();

	debug::debugDraw<Player, Player, Object, Actor>(this, "Debug For Class Player");
}

void Player::checkClashes(const float& time)
{
	isCollision = false;
	onGround = false;
	for (auto& i : obj)
	{
		auto playerRect = Rectangle::fromSfmlRect(sprite.getGlobalBounds());
		auto objectRect = Rectangle::fromSfmlRect(i.rect);
		if (i.name == "barrier")
		{
			if (sprite.getGlobalBounds().intersects(i.rect))
			{
				isCollision = true;
				isWalk = false;

				offset = Rectangle::GetIntersectionDepth(playerRect, objectRect).GetSfmlVector();

				auto copyOffset = offset;

				if (abs(copyOffset.x) > abs(copyOffset.y))
					copyOffset.x = 0;
				else
					copyOffset.y = 0;

				if (copyOffset.y < 0)
					onGround = true;

				position = position + copyOffset;
			}
		}
	}
}

void Engine::Player::handleEvent(sf::Event& e)
{
	if (inv.IsEnable())
		inv.handleEvent(e);
}
//
//Bullet* Engine::Player::ShootUpdate(Level& lvl)
//{
//	if (isShoot && !VStaticContainer::ShowDebugWindow && ImGui::GetIO().MetricsRenderWindows < 2)
//	{
//		auto item = inventory.getCurrItem<Gun>();
//		if (gunClock.getElapsedTime().asMilliseconds() > item->getRate())
//		{
//			gunClock.restart();
//			Image i;
//			i.loadFromFile("Data/images/bullet.png");
//			auto pos = sprite.getPosition();
//			pos.y -= 10;
//			pos.x += 5;
//			static int id;
//			++id;
//			return new Bullet(i, IntRect(0, 0, 16, 16), pos, "Bullet" + std::to_string(id), direction, item->getDamage(), lvl, name);
//		}
//	}
//	isShoot = false;
//	return nullptr;
//}
#include "serializer.h"

Player::~Player()
{
	save<Player, Actor, Object>(*this);
}
