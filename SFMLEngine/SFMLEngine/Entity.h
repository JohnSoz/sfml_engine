#pragma once
#include "DebugInteface.h"
#include "Level.h"
#include "Object.h"
#include "collider.h"
#include <utility>

namespace Engine
{
	class Entity : public Object
	{
	protected:
		sf::IntRect localRectangle;
		sf::FloatRect globalRectangle;
		sf::FloatRect debugRectangle;
		sf::FloatRect test;
		Collider collider;
		std::vector<ObjectLevel> obj;
		using Quad = std::pair<const sf::FloatRect*, const sf::FloatRect*>;

	public:
		Entity() = default;
		Entity(const Entity&) = default;
		Entity(sf::Vector2f position, std::string name);
		Entity(sf::Image& image, sf::Vector2f position, std::string name);
		Entity(sf::Image& image,
			sf::IntRect rect,
			sf::Vector2f position,
			std::string name);
		virtual ~Entity() {}

		virtual void update(float time) = 0;
		virtual void fixedUpdate() {}
		virtual void CollisionUpdate(Entity* objarray) {} // deprecated?
		void setLevel(Level& l) { obj = l.GetObjects("barrier"); }


		sf::FloatRect getRect() const { return sprite.getGlobalBounds(); } // return collider.getQuad().first;
		sf::IntRect getLocalRect() { return localRectangle; }
		Quad getDebugRect() const noexcept { return std::make_pair(&globalRectangle, &debugRectangle); } // return collider.getQuad();

		operator sf::Drawable& () { return sprite; }

		friend class ObjectHandler;
		friend auto meta::registerMembers<Entity>();
	};

	class Bullet final : public Entity
	{
	private:
		DirectionX dir;
		float damage;
		float speed = 0.3f;
		float scale = 0.2f;

		void CheckClashes();

	public:
		std::string shootersName;
		Bullet() = default;
		Bullet(sf::Image& image,
			sf::IntRect rect,
			sf::Vector2f position,
			std::string name,
			DirectionX directionX,
			float damage,
			std::string nameShooters);
		~Bullet() = default;

		void CollisionUpdate(Entity* entity) override;
		void update(float time) override;
		void fixedUpdate() override
		{
			debug::debugDraw<Object, Bullet>(this, getName());
		}
		friend auto meta::registerMembers<Bullet>();
	};
} // namespace Engine
namespace meta {
	template<>
	inline auto
		registerMembers<Engine::Entity>()
	{
		return members(member("localRectangle", &Engine::Entity::localRectangle),
			//member("collider", &Engine::Entity::collider),
			member("test", &Engine::Entity::test),
			member("globalRectangle", &Engine::Entity::globalRectangle),
			member("debugRectangle", &Engine::Entity::debugRectangle));
	}
	template<>
	inline auto
		registerMembers<Engine::Bullet>()
	{
		return members(member("speed", &Engine::Bullet::speed),
			member("direction", &Engine::Bullet::dir),
			member("damage", &Engine::Bullet::damage));
	}
} // namespace meta
