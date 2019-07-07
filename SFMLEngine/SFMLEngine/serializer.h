#pragma once
#include <fstream>
#include <MetaStuff/Meta.h>
#include <SFML/Graphics.hpp>
#include "json.hpp"
using json = nlohmann::json;

namespace sf
{
	template <typename T>
	void to_json(json& j, const T& obj);
}

namespace Engine
{
	inline std::string parseObjectName(std::string s)
	{
		auto eraseIter = s.find_last_of(':');
		s.erase(0, ++eraseIter);
		return s;
	}

	template <typename T>
	void to_json(json& j, const T& obj);

	template <typename Class,
		typename = std::enable_if_t<meta::isRegistered<Class>()>>
		json serialize(const Class & obj);

	template <typename Class,
		typename = std::enable_if_t<!meta::isRegistered<Class>()>,
		typename = void>
		json serialize(const Class & obj);

	template <typename Class>
	json serialize_basic(const Class& obj);

	template <typename T>
	json serialize_basic(const sf::Vector2<T>& obj);

	template <typename T>
	json serialize_basic(const sf::Rect<T>& obj);

	inline json serialize_basic(const sf::Sprite& obj)
	{
		json j;
		j["size"] = obj.getTextureRect();;
		return j;
	}

	template <typename T>
	void to_json(json& j, const T& obj)
	{
		if constexpr (std::is_enum<T>::value)
		{
			j = (int)obj;
		}
		else
			j = serialize(obj);
	}

	template <typename Class,
		typename>
		json serialize(const Class& obj)
	{
		json value;
		meta::doForAllMembers<Class>(
			[&](auto& member)
			{
				auto& valueName = value[member.getName()];
				if (member.canGetConstRef()) {
					valueName = member.get(obj);
				}
				else if (member.hasGetter()) {
					valueName = member.getCopy(obj);
				}
			}
		);
		return value;
	}

	template <typename Class,
		typename, typename>
		json serialize(const Class& obj)
	{
		return serialize_basic(obj);
	}

	template <typename Class>
	json serialize_basic(const Class& obj)
	{
		return json(obj);
	}

	template <typename type>
	json serialize_basic(const sf::Vector2<type>& obj)
	{
		json j;
		j["X"] = obj.x;
		j["Y"] = obj.y;
		return j;
	}

	template <typename T>
	json serialize_basic(const sf::Rect<T>& obj)
	{
		json j;
		j["left"] = obj.left;
		j["top"] = obj.top;
		j["width"] = obj.width;
		j["height"] = obj.height;
		return j;
	}

	template<class Base>
	void helper(json& j, Base& obj)
	{
		j = obj;
	}

	template<class T>
	void save(T& obj)
	{
		json j;
		helper<T>(j, obj);
		std::ofstream o("Data/save.json");
		o << std::setw(4) << j;
		o.close();

	}
}
namespace sf
{
	template <typename T>
	void to_json(json& j, const T& obj)
	{
		j = Engine::serialize(obj);
	}
}