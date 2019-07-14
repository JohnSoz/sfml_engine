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
		AnimationManager         animManager;
		sf::Clock                Pressclock;
		sf::Clock                actionClock;
		sf::Vector2f             originOffset = {};
		DirectionX               direction;
		DirectionY               directionY = DirectionY::Up;
		sf::RenderWindow*        window;

		bool  isWalk;
		bool  isCollision;
		bool  isShoot;
		float scale = 0.5;
		float CurrAngle = sprite.getRotation(), LastAngle;
		float time_actor;

		void updateSprite();
	public:
		float Radian;
		sf::Vector2f ray = { 0,0 };
		sf::Vector2f ray2 = { 0,0 };

		Actor(Level& lvl);
		Actor(sf::Vector2f POSITION, std::string NAME, sf::RenderWindow& w, Level& lvl, std::string_view animation);
		~Actor() {}

		virtual void handleEvent(sf::Event& e);
		virtual void isKeyPressed();
		virtual void checkClashes(float time);
		sf::Vector2f getOrigin() { return originOffset; }
		float getScale() { return scale; }
		void CollisionUpdate(Entity* entity) override;
		void RotateToMouse(float speed, sf::RenderWindow& window);
		void update(float time) override;

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