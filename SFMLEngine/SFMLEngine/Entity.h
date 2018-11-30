#pragma once
#include "Object.h"
#include "Level.h"
#include <utility>
namespace Engine
{
	class Entity : public Object
	{
	protected:
		sf::IntRect      localRectangle;
		sf::FloatRect    globalRectangle;
		sf::FloatRect    debugRectangle;
	public:
		Entity() = default;
		Entity(const Entity&) = default;
		Entity(Entity&&) = default;
		Entity(sf::Vector2f POSITION, std::string NAME);
		Entity(sf::Image& IMAGE, sf::Vector2f POSITION, std::string NAME);
		Entity(sf::Image& IMAGE, sf::IntRect r, sf::Vector2f pos, std::string name);
		virtual ~Entity();

		virtual void update(float time) { dw_o.draw(name); }
		virtual void start() {}
		void SetPos(int x, int y) { position.x = x; position.y = y; }

		void CollisionUpdate(std::vector<Entity*> objarray)
		{
			/*for (auto &i : objarray)
			{
				if (i->getName() != name)
					if (i->getSprite().getGlobalBounds().intersects(sprite.getGlobalBounds()))
						i->getNa
			}*/
		}

		sf::IntRect getRect() { return localRectangle; }
		const sf::Vector2f& getPos() { return position; }
		ObjectType getType() { return type; }

		std::pair<sf::FloatRect*, sf::FloatRect*> getDebugRect() { return std::make_pair(&globalRectangle, &debugRectangle); }

		friend class ObjectHandler;
	};

	class Test : public Entity
	{
	public:
		Test(sf::Image& IMAGE, sf::IntRect r, sf::Vector2f pos, std::string name) : Entity(IMAGE, r, pos, name)
		{
			hp = 100;
			sprite.setScale(0.5, 0.5);
			scale = 0.5;
			globalRectangle = sf::FloatRect(position.x, position.y, position.x + localRectangle.width * scale, position.y + localRectangle.height * scale);
			debugRectangle = sf::FloatRect(position.x + localRectangle.width * scale, position.y, position.x, position.y + localRectangle.height * scale);
		}
		void getDamage() { hp -= 1; }

		float scale;
		float hp;
	};

	class Bullet final : public Entity
	{
	private:
		float speed = 0.3;
		float angle;
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
		float scale = 0.2;
		DebugWindows<Bullet> db;
	public:
		Bullet() = default;
		~Bullet() = default;
		Bullet(sf::Image& IMAGE, sf::IntRect r, sf::Vector2f pos, std::string name, float Angle, float Damage, Level lvl) : Entity(IMAGE, r, pos, name)
		{
			angle = Angle;
			db.set(this);
			IsActive = true;
			obj = lvl.GetObjects("barrier");
			damage = Damage;
			sprite.setScale(scale, scale);
			sprite.setOrigin(r.width * scale, r.height * scale);
		}
		void update(float time) override
		{
			position.x += speed * std::cosf(angle) * time;
			position.y += speed * std::sinf(angle) * time;
			float posX = position.x + 1;
			float posY = position.y + 1;
			globalRectangle = sf::FloatRect(posX, posY, posX + localRectangle.width * scale, posY + localRectangle.height * scale);
			debugRectangle = sf::FloatRect(posX + localRectangle.width * scale, posY, posX, posY + localRectangle.height * scale);
			CheckClashes();
			sprite.setPosition(position.x + localRectangle.width * scale / 2, position.y + localRectangle.width * scale / 2);
			dw_o.draw("Object", true);
			db.draw("Bullet");
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
			member("angle", &Engine::Bullet::angle),
			member("damage", &Engine::Bullet::damage)
		);
	}
}