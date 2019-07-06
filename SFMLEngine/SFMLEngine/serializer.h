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

	template <typename T>
	void from_json(const json& j, T& obj);
}
namespace Engine
{

	inline std::string parseObjectName(std::string s)
	{
		auto eraseIter = s.find_last_of(':');
		s.erase(0, ++eraseIter);
		return s;
	}
	//----------------------------deserialize----------------------------
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
		//auto object = j_obj["Test"][parseObjectName(typeid(Class).name())];
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
						std::cout << typeid(MemberT).name();
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
	//----------------------------serialize----------------------------
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
		//TODO: TextureManager
		//j["path"] = 0;
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
		//auto& jsonToSave = j[obj.getName()];
		helper<T>(j, obj);
		//(helper<Types>(jsonToSave[parseObjectName(typeid(Types).name())], obj), ...);
		std::ofstream o("Data/save.json");
		o << std::setw(4) << j;
		o.close();

	}

	template<class T>
	void load_obj(T& obj)
	{
		json j2;
		std::ifstream i;
		i.open("Data/save.json");
		i >> j2;
		deserialize<T>(obj, j2);
		//(deserialize<Types>(obj, j2), ...);
		i.close();
	}

}
namespace sf
{
	template <typename T>
	void to_json(json& j, const T& obj)
	{
		j = Engine::serialize(obj);
	}
	template<typename T>
	void from_json(const json& j, T& obj)
	{
		Engine::deserialize(obj, j);
	}
}