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
	class Actor;

	class DebugWindow
	{
	private:
		void ShowHelpMarker(const char* desc);
		ImVec2 size = { 400,400 };
		std::vector<Object*> objectInf;
	public:
		bool isSelected;
		void pushObjectToDebug(Object& o) { objectInf.push_back(&o); }
		void actorInfo(bool *open, Actor& a);
		void objectInfo(bool *open, Object& a);
		void draw(bool *open)
		{
			for (auto i : objectInf)
			{
				auto z = static_cast<Entity*>(i);
				switch (i->type)
				{
				case OActor:
					objectInfo(open, *i);
					actorInfo(open, *(Actor*)(z));
					break;
				case None:
					objectInfo(open, *i);
					break;
				}
			}
		}
	};

	sf::Vector2f operator + (const sf::Vector2f& rect, float scale);
	class Actor : public Entity
	{
	protected:
		std::vector<ObjectLevel> obj;
		sf::Vector2f velocity;
		Inventory inventory;
		AnimationManager animManager;
		DebugWindows<Actor> dw_a;
		InventoryMenu inv;

		bool isWalk;
		bool isCollision;
		bool isShoot;

		float lives, armor;
		float speed, energy, friction, maxSpeed;
		float scale = 0.5;
		float CurrAngle = sprite.getRotation(), LastAngle;
		float time_actor;

		sf::Clock Pressclock;
		sf::Clock gunClock;
		sf::Vector2f originOffset = {};
		Direction direction = Direction::State;
		sf::RenderWindow* window;
		sf::Transformable PointOfFire;

		void updateSprite();

	public:
		float Radian;
		~Actor() = default;
		Actor() = delete;
		Actor(sf::Image& IMAGE, sf::Vector2f POSITION, std::string NAME, sf::RenderWindow& w, Level& lvl);

		sf::Vector2f getPointOfFire() { return PointOfFire.getPosition(); }

		virtual void handleEvent(sf::Event& e);

		void isKeyPressed();

		void checkClashes(float time);

		void CollisionUpdate(Entity* entity) override;

		void RotateToMouse(float speed, sf::RenderWindow& window);

		Engine::Bullet* shotUpdate(Level& lvl);

		void draw() { inv.draw(); }

		void update(float time) override;

		void start() override;

		void getDamage(float dmg);

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
			member("direction", &Engine::Actor::direction),
			member("velocity", &Engine::Actor::velocity),
			member("friction", &Engine::Actor::friction),
			member("CurrAngle", &Engine::Actor::CurrAngle),
			member("maxSpeed", &Engine::Actor::maxSpeed),
			member("energy", &Engine::Actor::energy)
		);
	}
}