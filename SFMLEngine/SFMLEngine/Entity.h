#pragma once
#include "Object.h"
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

		virtual void update(float time) = 0;
		void SetPos(int x, int y) { position.x = x; position.y = y; }

		sf::IntRect getRect() { return localRectangle; }
		const sf::Vector2f& getPos() { return position; }
		ObjectType getType() { return type; }

		std::pair<sf::FloatRect*, sf::FloatRect*> getDebugRect() { return std::make_pair(&globalRectangle, &debugRectangle); }

		friend class ObjectHandler;
	};
}