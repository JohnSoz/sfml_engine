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
		float lives, armor;
		float energy, friction, maxSpeed;
		float speedX, speedY;
		bool onGround;

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
			lives = armor = 50;
			speedX = speedY = 0;
			energy = 0.003f;
			friction = 0.005f;
			maxSpeed = 0.2f;
			onGround = true;
		}

		void isKeyPressed();
		void RotateToMouse(float speed, RenderWindow& w);
		void update(float time) override;

		void start() override
		{
		}

		void draw()
		{
			inv.draw();
		}

		void CollisionUpdate(Entity* entity) override
		{
		}

		void checkClashes(const float& time);
		void handleEvent(sf::Event& e) override;

		Bullet* ShootUpdate(Level& lvl)
		{
			if (isShoot && !VStaticContainer::ShowDebugWindow && ImGui::GetIO().MetricsRenderWindows < 2)
			{
				auto item = inventory.getCurrItem<Gun>();
				if (gunClock.getElapsedTime().asMilliseconds() > item->getRate())
				{
					gunClock.restart();
					Image i;
					i.loadFromFile("Data/images/bullet.png");
					isShoot = false;
					auto pos = sprite.getPosition();
					pos.y -= 10;
					pos.x += 5;
					static int id;
					++id;
					return new Bullet(i, IntRect(0, 0, 16, 16), pos, "Bullet" + std::to_string(id), direction, item->getDamage(), lvl, name);
				}
			}
			isShoot = false;
			return nullptr;
		}

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
			member("pos",      &Engine::Player::position),
			member("isWalk", &Engine::Player::isWalk),
			member("isCollision", &Engine::Player::isCollision),
			member("energy",   &Engine::Player::energy),
			member("offset", &Engine::Player::offset),
			member("speedX",   &Engine::Player::speedX),
			member("speedY",   &Engine::Player::speedY));
	}
} // namespace meta
