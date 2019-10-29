#pragma once
#include "Actor.h"
#include "EngineEvents.h"
#include "JsonLoader.h"
#include "HUD.h"

namespace Engine
{
	class Player;

	struct save_data
	{
		sf::Vector2f position;
		std::string  pathToAnimation;
		std::string  name;
		save_data& operator=(const Player& p);
	};

	class Player : public Actor
	{
	private:
		sf::Sprite    handSprite;
		sf::Vector2f  velocity;
		Inventory     inventory;
		sf::Vector2f  offset;
		HUD           hud;
		save_data     data;
		InventoryMenu inv;
		
		float CurrAngle = sprite.getRotation(), LastAngle;
		float health, armor;
		float energy, friction, maxSpeed;
		float speedX, speedY;
		bool  isWalk;
		bool  isCollision;
		bool  isShoot;
		bool  onGround;
		bool  isInvAction;
		bool  isJump;
	public:
		Player(sf::RenderWindow& window, Level& lvl, std::string_view pathToSave = "Data/save.json");
		Player(sf::Vector2f position, std::string name, RenderWindow& window, Level& lvl, std::string_view animation);
		~Player();

		Player& operator=(const Player& p) = default;
		Player& operator=(const save_data& p);

		void update(float time) override;
		void fixedUpdate() override;
		void handleEvent(sf::Event& e) override;
		void CollisionUpdate(Entity* entity) override {}
		void checkClashes(float time) override;

		void draw();
		void isKeyPressed();
		void RotateToMouse(float speed, RenderWindow& w);
		void increaseHealth(int h) { health += h; }
		void inventoryAction();

		DirectionX getDirection() const { return direction; }
		const AnimationManager& getAnimManager() const { return animManager; }

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
			member("vel", &Engine::Player::velocity),
			member("isWalk", &Engine::Player::isWalk),
			member("isJump", &Engine::Player::isJump),
			member("energy", &Engine::Player::energy),
			member("health", &Engine::Player::health),
			member("offset", &Engine::Player::offset),
			member("speedX", &Engine::Player::speedX),
			member("speedY", &Engine::Player::speedY),
			member("onGround", &Engine::Player::onGround),
			member("friction", &Engine::Player::friction),
			member("maxSpeed", &Engine::Player::maxSpeed),
			member("isCollision", &Engine::Player::isCollision)
		);
	}
} // namespace meta
