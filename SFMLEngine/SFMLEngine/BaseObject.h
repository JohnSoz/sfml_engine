#pragma once
#include <string>
#include <MetaStuff/Meta.h>
namespace Engine
{
	/// Enumeration of possible types for objects
	enum ObjectType { None = 0, BObject, OBject, OEntity, OActor, OPlayer };
	class BaseObject
	{
	private:
		ObjectType type;
		std::string name;
		bool IsActive;
	protected:
		void setType(int t) { type = (ObjectType)t; }
		void setName(std::string n) { name = n; }
	public:
		BaseObject() { type = None; }
		BaseObject(std::string_view name);
		int  getType() const { return type; }
		void destroy() { IsActive = false; }
		bool isActive()                 const { return IsActive; }
		void setActive(bool f) { IsActive = f; }
		std::string getName()           const { return name; }

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
