#pragma once

#include <SFML/Graphics.hpp>
#include "LogConsole.h"
#include <vector>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <iostream>
#include <MetaStuff/Meta.h>
#include "staticVariable.h"
#include "json.hpp"
#include "serializer.h"
using json = nlohmann::json;
using namespace nlohmann;
namespace Engine
{
	/// Enumeration of possible types for objects
	enum ObjectType { None = 0, OEntity, OPawn, OActor };
	/// Enumeration of the possible directions along the x-axis
	enum DirectionX { Left = 1, Right };
	/// Enumeration of the possible directions along the y-axis
	enum DirectionY { Up = 1, onGround };

	class Object
	{
	protected:
		ObjectType type;
		sf::Vector2f position; //<Object position in game coordinates
		sf::Texture texture; //<The texture of the current object
		sf::Sprite sprite; //<Sprite of the current object
		std::string name; //<Current object name /warning Must be unique
		bool IsActive; //<The flag indicates whether the current object is alive /warning If the value is false, the object is removed from the game world
	public:
		Object();
		explicit Object(std::string);
		Object(sf::Vector2f pos, std::string name);
		Object(sf::Vector2f pos, ObjectType t, std::string name);
		Object(sf::Image img, sf::Vector2f pos, ObjectType t, std::string name);
		~Object() = default;

		bool isActive()                 const { return IsActive; }
		std::string getName()           const { return name; }
		const sf::Sprite& getSprite()   const { return sprite; }
		const sf::Texture& getTexture() const { return texture; }
		void destroy() { IsActive = false; }

		friend bool operator==(const Object& obj, const Object& obj2);

		friend class DebugWindow;
		friend auto meta::registerMembers<Object>();
	};

}

namespace meta
{
	template <>
	inline auto registerMembers<Engine::Object>()
	{
		return members(
			member("type", &Engine::Object::type),
			member("name", &Engine::Object::name),
			member("sprite", &Engine::Object::sprite),
			member("IsActive", &Engine::Object::IsActive),
			member("position", &Engine::Object::position)
		);
	}
}