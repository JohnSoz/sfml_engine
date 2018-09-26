#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <stdio.h>
namespace Engine
{
	enum ObjectType { None = 0, OEntity, OPawn, OActor };
	enum Direction { Up = 1, Down, Left, Right, State };

	class Object
	{
	protected:
		ObjectType type;
		sf::Vector2f position;
		sf::Texture texture;//?
		sf::Sprite sprite;//?

		std::string name;
		bool IsActive;
	public:
		Object() = default;
		~Object() = default;
		Object(sf::Vector2f pos, std::string name);
		Object(sf::Vector2f pos, ObjectType t, std::string name);
		bool isActive() const { return IsActive; }
		void destroy() { IsActive = false; }
		std::string getName() { return name; }
		const sf::Sprite& GetSprite() { return sprite; }
		friend class Debug_Object;
	};
}

