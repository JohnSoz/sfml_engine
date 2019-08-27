#pragma once
#include <fstream>
#include <MetaStuff/Meta.h>
#include <SFML/Graphics.hpp>
#include "json.hpp"
using json = nlohmann::json;
namespace sf
{
	template <typename T>
	void from_json(const json& j, T& obj);
}

namespace Engine
{
	template <typename T>
	void from_json(const json& j, T& obj)
	{
		deserialize(obj, j);
	}

	template <typename Class>
	Class deserialize(const json& obj)
	{
		Class c;
		deserialize(c, obj);
		return c;
	}

	template <typename Class,
		typename = std::enable_if_t<meta::isRegistered<Class>()>>
		void deserialize(Class & obj, const json & j_obj)
	{
		meta::doForAllMembers<Class>(
			[&obj, &j_obj](auto& member)
			{
				auto& objName = j_obj[member.getName()];
				if (!objName.is_null()) {
					using MemberT = meta::get_member_type<decltype(member)>;
					if (member.hasSetter())
					{
						member.set(obj, objName.template get<MemberT>());
					}
					else if (member.canGetRef())
					{
						member.getRef(obj) = objName.template get<MemberT>();
					}
				}
			}
		);
	}
	template <typename Class,
		typename = std::enable_if_t<!meta::isRegistered<Class>()>,
		typename = void>
		void deserialize(Class & obj, const json & object)
	{
		obj = object.get<Class>();
	}

	template <typename T>
	void deserialize(sf::Vector2<T>& obj, const json& object)
	{
		obj.x = object.at("X").get<T>();
		obj.y = object.at("Y").get<T>();
	}

	template <typename T>
	void deserialize(sf::Rect<T>& obj, const json& object)
	{
		obj.height = object["height"].get<T>();
		obj.width = object["width"].get<T>();
		obj.left = object["left"].get<T>();
		obj.top = object["top"].get<T>();
	}

	template<class T>
	void load_obj(T& obj, std::string_view path)
	{
		json j2;
		std::ifstream i;
		i.open(path.data());
		i >> j2;
		deserialize<T>(obj, j2);
		i.close();
		Console::AppLog::addLog("Game is loaded(path:Data/save.json)", Console::system);
	}
}

namespace sf
{
	template<typename T>
	void from_json(const json& j, T& obj)
	{
		Engine::deserialize(obj, j);
	}
}