#include <entityx/entityx.h>
#include "Player.h"
#include "serializer.h"
#include "deserializer.h"
#include <boost/math/constants/constants.hpp>
using namespace Engine;

Engine::Player::Player(sf::RenderWindow& _window, Level& _lvl, std::string_view _pathToSave) :
	Actor(_lvl),
	inv("Data/GUI/MyUI/MainMenu.txt", _window),
	hud("Data/GUI/MyUI/MainMenu.txt", _window, inventory)
{
	setType(ObjectType::OPlayer);
	EventManager::eventManager.subscribe<Events::Event_Inventory_UI>(inv);
	isWalk = isCollision = isShoot = isJump = isInvAction = false;
	onGround = true;
	inv.makeMenu(inventory);
	hud.makeHud();
	load_obj<save_data>(data, _pathToSave);
	*this = data;
	setTexture(animManager.texture);
	updateSprite();
	health = armor = 50;
	speedX = speedY = 0;
	energy = 0.003f;
	friction = 0.005f;
	maxSpeed = 0.2f;
}

Engine::Player::Player(sf::Vector2f _position, std::string _name, RenderWindow& _window, Level& lvl, std::string_view _animation)
	: Actor(_position, _name, _window, lvl, _animation),
	inv("Data/GUI/MyUI/MainMenu.txt", _window),
	hud("Data/GUI/MyUI/MainMenu.txt", _window, inventory)
{
	setType(ObjectType::OPlayer);
	EventManager::eventManager.subscribe<Events::Event_Inventory_UI>(inv);
	isWalk = isCollision = isShoot = isJump = isInvAction = false;
	onGround = true;
	inv.makeMenu(inventory);
	hud.makeHud();
	health = armor = 50;
	speedX = speedY = LastAngle = 0;
	energy = 0.003f;
	friction = 0.005f;
	maxSpeed = 0.2f;
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
		directionY = DirectionY::Up;
		onGround = false;
		velocity.y = -(energy * 160.f);
	}
	if (Keyboard::isKeyPressed(Keyboard::A))
	{
		isWalk = true;
		if (direction != DirectionX::Left)
			velocity.x -= velocity.x * 0.85f;
		direction = DirectionX::Left;
		velocity.x = (abs(velocity.x) < maxSpeed) ? velocity.x -= energy : velocity.x = -maxSpeed;
	}
	else if (Keyboard::isKeyPressed(Keyboard::D))
	{
		isWalk = true;
		if (direction != DirectionX::Right)
			velocity.x -= velocity.x * 0.85f;
		direction = DirectionX::Right;
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
	CurrAngle = (float)(atan2f(v, a)) * 180.f / boost::math::constants::pi<float>();

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

	float lastradian = LastAngle * boost::math::constants::pi<float>() / 180;
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

		position += velocity;
		checkClashes(time);
		sprite.setPosition(position);
		if (animManager.getCurrAnimation()->name != "Walk" && isWalk && onGround && !isJump)
		{
			//animManager.setCurrAnimation(animManager.getAnimationByName("fall"));
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
		sprite.setTextureRect(animManager.animUpdate(time));

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

		inventoryAction();
		inventory.update();
		updateSprite(); //???
	}
}

void Engine::Player::fixedUpdate()
{
	debug::debugDraw<Player, Object, Actor, Entity>(this, "Debug For Class Player");
	debug::debugDraw<Collider>(&this->collider, "Debug For Class Collider");
}

void Engine::Player::draw()
{
	inv.draw();
	hud.draw();
}

void Engine::Player::checkClashes(float time)
{
	isCollision = false;
	const sf::Vector2f pos = { position.x - originOffset.x * scale, position.y - localRectangle.height * scale };
	//const sf::Vector2f _pos =  { sprite.getPosition().x - sprite.getLocalBounds().width / 2, sprite.getPosition().y };
	// pos != _pos
	const sf::Vector2f pos1 = { pos.x + 0.1f, pos.y + localRectangle.height * scale - 0.1f };
	const sf::Vector2f pos2 = { pos.x + getRect().width - 0.1f, pos.y + localRectangle.height * scale - 0.1f };
	const sf::Vector2f pos3 = { pos.x + getRect().width / 2, pos.y + localRectangle.height * scale - 0.1f };

	left.setStart(pos1);
	left.setDist(10);
	left.setAngle(90);
	right.setStart(pos2);
	right.setDist(10);
	right.setAngle(90);
	middle.setStart(pos3);
	middle.setDist(5);
	middle.setAngle(90);

	//static float angle; //????????????????????????????????????
	//angle += (angle >= 360) ? 1 : -1;
	//const sf::Vector2f pos4 = { getPos().x, getPos().y - localRectangle.height * scale / 2 };
	//govno.setStart(pos4);
	//govno.setDist(100);
	//govno.setAngle(angle);

	for (const auto& i : obj)
	{
		//auto playerRect = Rectangle::fromSfmlRect(sf::FloatRect(getRect().left, position.y, getRect().width, getRect().height));
		const sf::Vector2f pos6 = { getPos().x - getLocalRect().width * getScale() / 2, getPos().y - getLocalRect().height * getScale() };
		auto sf_PLayerRect = sf::FloatRect(pos6.x, pos6.y, localRectangle.width * scale, localRectangle.height * scale); //why not used sprite.getGlobalBounds() ?
		auto playerRect2 = Rectangle::fromSfmlRect(sf_PLayerRect);
		auto objectRect = Rectangle::fromSfmlRect(i.rect);

		if (i.name == "barrier")
		{
			left.castTo(i);
			right.castTo(i);
			middle.castTo(i);
			govno.castTo(i);
			if ((left.hitDist() > 1.f && right.hitDist() > 1.f) || middle.hitDist() > 1.f)
				onGround = false;
			else
				onGround = true;

			if (sf_PLayerRect.intersects(i.rect))
			{
				isCollision = true;
				auto copyOffset = Rectangle::GetIntersectionDepth(playerRect2, objectRect).GetSfmlVector();
				test.left = copyOffset.x;
				test.top = copyOffset.y;
				if (copyOffset.y > 0)
					velocity.y = 0;
				if (copyOffset != sf::Vector2f(0, 0))
					position = position + copyOffset;
				else
					isCollision = false;
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
			isInvAction = !isInvAction;
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
	this->setType(ObjectType::OPlayer);
	this->position = p.position;
	this->animManager.path = p.pathToAnimation;
	this->animManager.loadAnimation_x(p.pathToAnimation, *window);
	return *this;
}