#pragma once
#include "Actor.h"
#include "EngineEvents.h"
#include "JsonLoader.h"
#include "HUD.h"

namespace Engine
{
	struct save_data
	{
		sf::Vector2f position;
		std::string pathToAnimation;
		std::string name;
	};

	class Player : public Actor
	{
	private:
		sf::Sprite handSprite;
		sf::Vector2f velocity;
		Inventory inventory;
		sf::Vector2f offset;

		InventoryMenu inv;
		float health, armor;
		float energy, friction, maxSpeed;
		float speedX, speedY;
		float grav = 0.005f;
		bool onGround;
		bool isInvAction;
		bool isJump;
		HUD hud;
	public:
		save_data data;
		Player(sf::RenderWindow& w, Level& lvl) :
			Actor(lvl),
			inv("Data/GUI/MyUI/MainMenu.txt", w),
			hud("Data/GUI/MyUI/MainMenu.txt", w, inventory)
		{
			type = OPlayer;
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
		void load_opa() //Костыль
		{
			*this = data;
			setTexture(animManager.texture);
			updateSprite();
		}
		Player(sf::Vector2f POSITION, std::string NAME, RenderWindow& w, Level& lvl, std::string_view animation)
			: Actor(POSITION, NAME, w, lvl, animation),
			inv("Data/GUI/MyUI/MainMenu.txt", w),
			hud("Data/GUI/MyUI/MainMenu.txt", w, inventory)
		{
			type = OPlayer;
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
		Player& operator=(const Player& p) = default;
		Player& operator=(const save_data& p)
		{
			this->name = p.name;
			this->position = p.position;
			this->animManager.path = p.pathToAnimation;
			this->animManager.LoadAnimation_x(p.pathToAnimation);
			return *this;
		}
		void isKeyPressed();
		void RotateToMouse(float speed, RenderWindow& w);
		void update(float time) override;

		void start() override {}

		void draw()
		{
			inv.draw();
			hud.draw();
		}

		void inventoryAction()
		{
			if (isInvAction)
			{
				inventory.getCurrItem<Item>()->action(*this);
			}
			isInvAction = false;
		}

		DirectionX getDirection() const { return direction; }
		void increaseHealth(int h) { health += h; }
		void CollisionUpdate(Entity* entity) override {}
		void checkClashes(const float& time);
		void handleEvent(sf::Event& e) override;

		~Player();

		friend auto meta::registerMembers<Player>();
	};
} // namespace Engine
namespace meta
{
	template <>
	inline auto registerMembers<Engine::save_data>()
	{
		return members(
			member("position", &Engine::save_data::position),
			member("animation", &Engine::save_data::pathToAnimation),
			member("name", &Engine::save_data::name)
		);
	}
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
			member("maxSpeed", &Engine::Player::maxSpeed),
			member("speedY", &Engine::Player::speedY),
			member("vel", &Engine::Player::velocity)
		);
	}
} // namespace meta
