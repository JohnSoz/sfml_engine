#include <entityx/entityx.h>
#include "Player.h"
#include "serializer.h"
#include "deserializer.h"
using namespace Engine;

Engine::Player::Player(sf::RenderWindow& w, Level& lvl, std::string_view pathToSave) :
	Actor(lvl),
	inv("Data/GUI/MyUI/MainMenu.txt", w),
	hud("Data/GUI/MyUI/MainMenu.txt", w, inventory)
{
	setType(OPlayer);
	EventManager::eventManager.subscribe<Events::Event_Inventory_UI>(inv);
	isWalk = isCollision = isShoot = false;
	onGround = true;
	inv.makeMenu(inventory);
	hud.makeHud();
	health = armor = 50;
	speedX = speedY = 0;
	energy = 0.003f;
	friction = 0.005f;
	maxSpeed = 0.2f;
	onGround = true;
	isInvAction = false;
	load_obj<save_data>(data, pathToSave);
	*this = data;
	setTexture(animManager.texture);
	updateSprite();
}

Engine::Player::Player(sf::Vector2f POSITION, std::string NAME, RenderWindow& w, Level& lvl, std::string_view animation)
	: Actor(POSITION, NAME, w, lvl, animation),
	inv("Data/GUI/MyUI/MainMenu.txt", w),
	hud("Data/GUI/MyUI/MainMenu.txt", w, inventory)
{
	setType(OPlayer);
	EventManager::eventManager.subscribe<Events::Event_Inventory_UI>(inv);
	isWalk = isCollision = isShoot = false;
	onGround = true;
	inv.makeMenu(inventory);
	hud.makeHud();
	health = armor = 50;
	speedX = speedY = 0;
	energy = 0.003f;
	friction = 0.005f;
	maxSpeed = 0.2f;
	onGround = true;
	isInvAction = false;
}

Player::~Player()
{
	data = *this;
	save<save_data>(this->data, "Data/save.json");
}

void Player::isKeyPressed()
{
	if (Keyboard::isKeyPressed(Keyboard::W) && onGround)
	{
		isWalk = false;
		isJump = true;
		directionY = Up;
		onGround = false;
		velocity.y = -(energy * 150.f);
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
		isWalk = false;

	if (Keyboard::isKeyPressed(Keyboard::I))
		if (Pressclock.getElapsedTime().asMilliseconds() > 500)
		{
			hud.update();
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
	if (time)
	{
		switch (direction)
		{
		case DirectionX::Left:
			sprite.setScale(scale * -1, scale);
			break;
		case DirectionX::Right:
			sprite.setScale(scale, scale);
			break;
		}

		if (velocity.y < 0)
			isJump = true;

		if (!onGround)
			velocity.y += energy / 2;
		else
		{
			isJump = false;
			velocity.y = 0;
		}

		if (!isWalk)
			(velocity.x > 0) ? velocity.x -= friction : velocity.x = 0;

		position += velocity * time;
		checkClashes(time);
		sprite.setPosition(position);

		if (isWalk && animManager.getCurrAnimation()->name != "Walk" && onGround)
		{
			animManager.setCurrAnimation(animManager.getAnimationByName("Walk"));
		}
		else if (!onGround && animManager.getCurrAnimation()->name != "Jump" && isJump)
		{
			animManager.setCurrAnimation(animManager.getAnimationByName("Jump"));
		}
		else if (!onGround && animManager.getCurrAnimation()->name != "fall" && !isJump)
		{
			animManager.setCurrAnimation(animManager.getAnimationByName("fall"));
		}
		else if (!isWalk && animManager.getCurrAnimation()->name != "Idle" && onGround)
		{
			animManager.setCurrAnimation(animManager.getAnimationByName("Idle"));
		}
		updateSprite();
		sprite.setTextureRect(animManager.animUpdate(time));
		
		inventoryAction();
		inventory.update();

		localRectangle = sprite.getTextureRect();		
		const sf::Vector2f pos = { position.x - originOffset.x * scale, position.y - localRectangle.height * scale };
		collider.setPosition(pos);	
		collider.setSize(sf::Vector2f(localRectangle.width, localRectangle.height));
		globalRectangle = sf::FloatRect(pos.x, pos.y,
			pos.x + localRectangle.width * scale,
			pos.y + localRectangle.height * scale);

		debugRectangle = sf::FloatRect(pos.x + localRectangle.width * scale,
			pos.y, pos.x,
			pos.y + localRectangle.height * scale);
	}
}

void Engine::Player::draw()
{
	inv.draw();
	hud.draw();
}

void Engine::Player::checkClashes(float time)
{
	isCollision = false;
	auto pos = sprite.getPosition();
	auto globBounds = sprite.getGlobalBounds();
	ray = { sprite.getPosition().x - getOrigin().x, sprite.getPosition().y + 2.2f };
	ray2 = { sprite.getPosition().x + getOrigin().x, sprite.getPosition().y + 2.2f };

	sf::Vector2f pos1 = { sprite.getPosition().x - getOrigin().x , sprite.getPosition().y - 1.f };
	sf::Vector2f pos2 = { sprite.getPosition().x + getOrigin().x , sprite.getPosition().y - 1.f };
	float dist = raycastLevelObject(pos1, { +1, 2.2 }, obj, "barrier");
	float dist2 = raycastLevelObject(pos2, { -1, 2.2 }, obj, "barrier");
	//std::cout << "dist = " << (float)(dist) << "   ___   " << "dist2 = " << (float)(dist2) << endl;

	if (dist < 1 && dist2 < 1)
		onGround = false;
	else
		onGround = true;

	for (const auto& i : obj)
	{
		auto playerRect = Rectangle::fromSfmlRect(sf::FloatRect(getRect().left, position.y, getRect().width, getRect().height));
		auto objectRect = Rectangle::fromSfmlRect(i.rect);
		if (i.name == "barrier")
		{
			if (sprite.getGlobalBounds().intersects(i.rect))
			{
				isCollision = true;
				auto copyOffset = Rectangle::GetIntersectionDepth2(playerRect, objectRect).GetSfmlVector();
				test.left = copyOffset.x;
				test.top = copyOffset.y;

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

void Engine::Player::inventoryAction()
{
	if (isInvAction)
		inventory.getCurrItem<Item>()->action(*this);
	isInvAction = false;
}

void Engine::Player::handleEvent(sf::Event& e)
{
	if (inv.IsEnable())
		inv.handleEvent(e);
	hud.handleEvent(e);
	if (e.type == sf::Event::KeyReleased)
	{
		if (e.key.code == sf::Keyboard::C)
		{
			isInvAction = !isInvAction;
		}
	}
}

save_data& Engine::save_data::operator=(const Player& p)
{
	this->name = p.getName();
	this->position = p.getPos();
	this->pathToAnimation = p.getAnimManager().path;
	return *this;
}

Player& Engine::Player::operator=(const save_data& p)
{
	this->setName(p.name);
	this->setType(OPlayer);
	this->position = p.position;
	this->animManager.path = p.pathToAnimation;
	this->animManager.loadAnimation_x(p.pathToAnimation, *window);
	return *this;
}