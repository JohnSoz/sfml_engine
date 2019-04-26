#pragma once
#include "Actor.h"
#include "EngineEvents.h"
#include "JsonLoader.h"

namespace Engine
{
	class Player : public Actor
	{
	private:
		sf::Sprite handSprite;
		sf::Vector2f velocity;
		Inventory inventory;
		sf::Vector2f vec;
		sf::Vector2f offset;
		InventoryMenu inv;
		float health, armor;
		float energy, friction, maxSpeed;
		float speedX, speedY;
		bool onGround;
		bool isInvAction;
	public:
		Player(Image& IMAGE, sf::Vector2f POSITION, std::string NAME, RenderWindow& w, Level& lvl)
			: Actor(IMAGE, POSITION, NAME, w, lvl),
			inv("Data/GUI/MyUI/MainMenu.txt", w)
		{
			vec.x = 12;
			EventManager::eventManager.subscribe<Events::Event_Inventory_UI>(inv);
			isWalk = isCollision = isShoot = false;
			onGround = true;
			inv.makeMenu(inventory);
			health = armor = 50;
			speedX = speedY = 0;
			energy = 0.003f;
			friction = 0.005f;
			maxSpeed = 0.2f;
			onGround = true;
			isInvAction = false;
		}

		void isKeyPressed();
		void RotateToMouse(float speed, RenderWindow& w);
		void update(float time) override;

		void start() override {}

		void draw()
		{
			inv.draw();
		}

		void inventoryAction()
		{
			if (isInvAction)
				inventory.getCurrItem<Item>()->action(*this);
			isInvAction = false;
		}
		DirectionX getDirection() const { return direction; }
		void CollisionUpdate(Entity* entity) override {}
		void checkClashes(const float& time);
		void handleEvent(sf::Event& e) override;
		//Bullet* ShootUpdate(Level& lvl);

		~Player();

		friend auto meta::registerMembers<Player>();
	};
} // namespace Engine
namespace meta
{
	template <>
	inline auto registerMembers<Engine::Player>()
	{
		return members(
			member("onGround", &Engine::Player::onGround),
			member("friction", &Engine::Player::friction),
			member("energy", &Engine::Player::energy),
			member("health", &Engine::Player::health),
			member("offset", &Engine::Player::offset),
			member("speedX", &Engine::Player::speedX),
			member("speedY", &Engine::Player::speedY));
	}
} // namespace meta
