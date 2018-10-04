#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <stdio.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <MetaStuff/Meta.h>
using namespace meta;
namespace Engine
{
	enum ObjectType { None = 0, OEntity, OPawn, OActor };
	enum Direction { Up = 1, Down, Left, Right, State };

	template<class T>
	class DebugWindow
	{
	private:
		T* ptr;
	public:
		DebugWindow()
		{
		}
		void setObjectPtr(T& obj) { ptr = obj; }
		void draw() {}
		void getInfo() { ptr->getName(); }
	};

	class Object
	{
	protected:
		ObjectType type;
		sf::Vector2f position;
		sf::Texture texture;
		sf::Sprite sprite;
		std::string name;
		bool IsActive;
	public:
		Object() { type = ObjectType::None; }
		Object(std::string);
		~Object() = default;
		Object(sf::Vector2f pos, std::string name);
		Object(sf::Vector2f pos, ObjectType t, std::string name);
		bool isActive() const { return IsActive; }
		void destroy() { IsActive = false; }
		std::string getName() { return name; }
		const sf::Sprite& GetSprite() { return sprite; }
		friend class Debug_Object;
		friend auto meta::registerMembers<Object>();
	};

}


namespace meta
{
	template <>
	inline auto registerMembers<Engine::Object>()
	{
		return members(
			member("name", &Engine::Object::name),
			member("type", &Engine::Object::type),
			member("texture", &Engine::Object::texture),
			member("sprite", &Engine::Object::sprite),
			member("position", &Engine::Object::position),
			member("IsActive", &Engine::Object::IsActive)
		);
	}
}