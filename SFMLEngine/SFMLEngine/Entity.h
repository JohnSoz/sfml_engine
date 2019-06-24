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
		typedef std::pair<const sf::FloatRect*, const sf::FloatRect*> Quad;

	public:
		Entity() = default;
		Entity(const Entity&) = default;
		Entity(sf::Vector2f POSITION, std::string NAME);
		Entity(sf::Image& IMAGE, sf::Vector2f POSITION, std::string NAME);
		Entity(sf::Image& IMAGE, sf::IntRect r, sf::Vector2f pos, std::string name);
		virtual ~Entity();

		virtual void update(float time) {}
		virtual void start() {}
		virtual void CollisionUpdate(Entity* objarray) {}
		void SetPos(int x, int y) { position.x = x; position.y = y; }

		sf::FloatRect getRect() const { return sprite.getGlobalBounds(); }
		sf::Vector2f  getPos()  const { return position; }
		ObjectType    getType() const { return type; }

		const Quad& getDebugRect() const noexcept { return std::make_pair(&globalRectangle, &debugRectangle); }

		friend class ObjectHandler;
	};

	class Bullet final : public Entity
	{
	private:
		float speed = 0.3f;
		DirectionX dir;
		float damage;
		std::vector<ObjectLevel> obj;
		void CheckClashes()
		{
			auto objRect = sf::FloatRect(position.x, position.y, localRectangle.width * scale, localRectangle.height * scale);
			for (auto& o : obj)
			{
				if (objRect.intersects(o.rect))
				{
					IsActive = false;
				}
			}
		}
		float scale = 0.2f;
	public:
		std::string shootersName;
		Bullet() = default;
		~Bullet() = default;
		Bullet(sf::Image& IMAGE, sf::IntRect r, sf::Vector2f pos, std::string name, DirectionX d, float Damage, Level lvl, std::string nameShooters) : Entity(IMAGE, r, pos, name)
		{
			shootersName = nameShooters;
			dir = d;
			IsActive = true;
			obj = lvl.GetObjects("barrier");
			damage = Damage;
			sprite.setScale(scale, scale);
			sprite.setOrigin(r.width * scale, r.height * scale);
		}
		void CollisionUpdate(Entity* entity) override
		{
			std::cout << name;
		}
		void update(float time) override
		{
			if (dir == Left)
			{
				position.x -= speed * time;
			}
			else
				position.x += speed * time;

			float posX = position.x + 1;
			float posY = position.y + 1;
			globalRectangle = sf::FloatRect(posX, posY, posX + localRectangle.width * scale, posY + localRectangle.height * scale);
			debugRectangle = sf::FloatRect(posX + localRectangle.width * scale, posY, posX, posY + localRectangle.height * scale);
			CheckClashes();
			sprite.setPosition(position.x + localRectangle.width * scale / 2, position.y + localRectangle.width * scale / 2);
			debug::debugDraw<Bullet, Object, Bullet>(this, name);
		}
		friend auto meta::registerMembers<Bullet>();
	};


}
namespace meta
{
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