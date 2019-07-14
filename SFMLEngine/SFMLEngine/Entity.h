#pragma once
#include "Object.h"
#include "Level.h"
#include <utility>
#include "DebugInteface.h"

namespace Engine
{
	class Entity : public Object
	{
	protected:
		sf::IntRect      localRectangle;
		sf::FloatRect    globalRectangle;
		sf::FloatRect    debugRectangle;
		sf::FloatRect    test;
		std::vector<ObjectLevel> obj;
		using Quad = std::pair<const sf::FloatRect*, const sf::FloatRect*>;
		
	public:
		Entity() = default;
		Entity(const Entity&) = default;
		Entity(sf::Vector2f POSITION, std::string NAME);
		Entity(sf::Image& IMAGE, sf::Vector2f POSITION, std::string NAME);
		Entity(sf::Image& IMAGE, sf::IntRect r, sf::Vector2f pos, std::string name);
		virtual ~Entity();

		virtual void update(float time) {}
		virtual void CollisionUpdate(Entity* objarray) {}
		virtual void setLevel(Level& l) { obj = l.GetObjects("barrier"); }
		void SetPos(int x, int y) { position.x = x; position.y = y; }

		sf::FloatRect getRect() const { return sprite.getGlobalBounds(); }
		sf::Vector2f  getPos()  const { return position; }
		ObjectType    getType() const { return type; }
		Quad getDebugRect() const noexcept { return std::make_pair(&globalRectangle, &debugRectangle); }

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
		Bullet(sf::Image& IMAGE, sf::IntRect r, sf::Vector2f pos, std::string name, DirectionX d, float Damage, std::string nameShooters);
		~Bullet() = default;
		
		void CollisionUpdate(Entity* entity) override;
		void update(float time) override;

		friend auto meta::registerMembers<Bullet>();
	};


}
namespace meta
{
	template <>
	inline auto registerMembers<Engine::Entity>()
	{
		return members(
			member("localRectangle", &Engine::Entity::localRectangle),
			member("intersec", &Engine::Entity::test),
			member("globalRectangle", &Engine::Entity::globalRectangle),
			member("debugRectangle", &Engine::Entity::debugRectangle)
		);
	}
	template <>
	inline auto registerMembers<Engine::Bullet>()
	{
		return members(
			member("speed", &Engine::Bullet::speed),
			member("direction", &Engine::Bullet::dir),
			member("damage", &Engine::Bullet::damage)
		);
	}
}