#pragma once
#include "Entity.h"
#include "AnimationManager.h"
#include "Inventory.h"
#include <MetaStuff/Meta.h>
#include "JsonLoader.h"
#include "InventoryMenu.h"

using namespace meta;
namespace Engine
{
	sf::Vector2f operator + (const sf::Vector2f& rect, float scale);
	class Actor : public Entity
	{
	protected:
		std::vector<ObjectLevel> obj;
		//sf::Vector2f velocity;
		//Inventory inventory;
		//AnimationManager animManager;
		AnimationManager animManager;
		//InventoryMenu inv;
		//DebugWindows<Actor> dw_a;

		bool isWalk;
		bool isCollision;
		bool isShoot;

		//float lives, armor;
		//float speed, energy, friction, maxSpeed;
		float scale = 0.5;
		float CurrAngle = sprite.getRotation(), LastAngle;
		float time_actor;
		sf::Clock Pressclock;
		sf::Clock actionClock;
		sf::Vector2f originOffset = {};
		DirectionX direction;
		DirectionY directionY = DirectionY::Up;
		sf::RenderWindow* window;
		//sf::Transformable PointOfFire;

		void updateSprite();

	public:
		float Radian;
		virtual ~Actor() {}
		Actor() = delete;
		Actor(sf::Vector2f POSITION, std::string NAME, sf::RenderWindow& w, Level& lvl, std::string_view animation);

		virtual void handleEvent(sf::Event& e);

		virtual void isKeyPressed();

		virtual void checkClashes(float time);

		void CollisionUpdate(Entity* entity) override;

		void RotateToMouse(float speed, sf::RenderWindow& window);

		void update(float time) override;

		void start() override;

		friend class DebugWindow;
		friend auto meta::registerMembers<Engine::Actor>();
	};
}
namespace meta
{
	template <>
	inline auto registerMembers<Engine::Actor>()
	{
		return members(
			member("isCollision", &Engine::Actor::isCollision),
			member("isWalk", &Engine::Actor::isWalk),
			member("originOffset", &Engine::Actor::originOffset)
		);
	}
}