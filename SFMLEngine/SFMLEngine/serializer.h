#pragma once
#include <MetaStuff/Meta.h>
#include "json.hpp"
#include <SFML/Graphics/Vertex.hpp>
#include <fstream>
using json = nlohmann::json;
using namespace nlohmann;
namespace nlohmann
{
	template <typename Class,
		typename = std::enable_if_t <meta::isRegistered<Class>()>>
		json serialize(const Class & obj);

	template <typename Class,
		typename = std::enable_if_t <!meta::isRegistered<Class>()>,
		typename = void>
		json serialize(const Class & obj);

	template <typename Class>
	json serialize_basic(const Class& obj);

	template <typename T>
	json serialize_basic(const sf::Vector2<T>& obj);

	template <typename T>
	void to_json(json& j, const sf::Vector2<T>& obj) { j = serialize(obj); }

	template <typename Class,
		typename>
		json serialize(const Class& obj)
	{
		json value;
		meta::doForAllMembers<Class>(
			[&obj, &value](auto & member)
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
}

namespace Engine
{

	template <typename T>
	void to_json(json& j, const T& obj)
	{
		j = serialize(obj);
	}

	template<class T>
	void save(T& obj)
	{
		json j;
		j = obj;
		std::ofstream o("Data/save.json");
		o << std::setw(4) << j;
		o.close();
	}

}