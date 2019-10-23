#pragma once
#include <string>
#include <MetaStuff/Meta.h>
namespace Engine
{
	/// Enumeration of possible types for objects
	enum class ObjectType { None = 0, BObject, OBject, OEntity, OActor, OPlayer };
	/// Enumeration of the possible directions along the x-axis
	enum class DirectionX { Left = 1, Right };
	/// Enumeration of the possible directions along the y-axis
	enum class DirectionY { Up = 1, onGround };

	class BaseObject
	{
	private:
		ObjectType type;
		std::string name;
		bool IsActive;
	protected:
		void setType(ObjectType _type) { type = _type; }
		void setName(std::string _name) { name = _name; }
	public:
		BaseObject() : type(ObjectType::None), IsActive(true) {}
		BaseObject(std::string_view);

		void destroy() { IsActive = false; }

		bool isActive()       const { return IsActive; }
		ObjectType  getType() const { return type; }
		std::string getName() const { return name; }

		friend bool operator==(const BaseObject& obj, const BaseObject& obj2);
		friend auto meta::registerMembers<BaseObject>();
	};
}
namespace meta
{
	template <>
	inline auto registerMembers<Engine::BaseObject>()
	{
		return members(
			member("type", &Engine::BaseObject::setType),
			member("type", &Engine::BaseObject::getType),
			member("name", &Engine::BaseObject::name),
			member("IsActive", &Engine::BaseObject::IsActive)
		);
	}
}
