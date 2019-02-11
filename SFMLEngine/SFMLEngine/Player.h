#pragma once
#include "Actor.h"
#include <MetaStuff/Meta.h>
namespace Engine
{
	class Player : public Actor
	{
	private:
		sf::Sprite handSprite;
		sf::Vector2f velocity;
		Inventory inventory;

		InventoryMenu inv;

		float lives, armor;
		float energy, friction, maxSpeed;
		float speedX, speedY;
		bool onGround;
		//sf::Vector2f velocity;
		//Inventory inventory;
		//AnimationManager animManager;

		//bool isWalk;
		//bool isCollision;
		//bool isShoot;

		//bool onGround;

		//float lives, armor;
		//float speed, energy, friction, maxSpeed;
		//float scale = 0.5;
		//float CurrAngle = sprite.getRotation(), LastAngle;

		/*Direction state = Direction::State;
			sf::Transformable PointOfFire;*/
		DebugWindows<Player> dp;

	public:
		Player(sf::Image& IMAGE, sf::Vector2f POSITION, std::string NAME, sf::RenderWindow& w, Level& lvl)
			: Actor(IMAGE, POSITION, NAME, w, lvl),
			inv("Data/GUI/MyUI/MainMenu.txt", w)
		{
			isWalk = isCollision = isShoot = false;
			onGround = true;
			inv.makeMenu(inventory);
			lives = armor = 50;
			speedX = speedY = 0;
			energy = 0.001;
			friction = 0.005;
			maxSpeed = 0.2;
			dp.set(this);
			onGround = true;
		}
		void isKeyPressed();
		void RotateToMouse(float speed, sf::RenderWindow& w);
		void update(float time) override;
		void start() override {}
		void draw() { inv.draw(); }
		void CollisionUpdate(Entity* entity) override {}
		void checkClashes(float time);
		void handleEvent(sf::Event& e) override;
		Bullet* ShootUpdate(Level& lvl)
		{
			if (isShoot && !Engine::VStaticContainer::ShowDebugWindow&& ImGui::GetIO().MetricsRenderWindows < 2)
			{
				auto item = inventory.getCurrItem<Gun>();
				if (gunClock.getElapsedTime().asMilliseconds() > item->getRate())
				{
					gunClock.restart();
					sf::Image i;
					i.loadFromFile("Data/images/bullet.png");
					isShoot = false;
					auto pos = sprite.getPosition();
					pos.y -= 10;
					pos.x += 5;
					return new Engine::Bullet(i, sf::IntRect(0, 0, 16, 16), pos, "Bullet", direction, item->getDamage(), lvl, name);
				}
			}
			isShoot = false;
			return nullptr;
		}
		~Player() = default;
		friend auto meta::registerMembers<Engine::Player>();
	};
} // namespace Engine
namespace meta
{
	template <>
	inline auto registerMembers<Engine::Player>()
	{
		return members(
			member("velocity", &Engine::Player::velocity),
			member("onGround", &Engine::Player::onGround),
			member("friction", &Engine::Player::friction),
			member("localrect", &Engine::Player::localRectangle),
			member("energy", &Engine::Player::energy),
			member("speedX", &Engine::Player::speedX),
			member("speedY", &Engine::Player::speedY));
	}
} // namespace meta
