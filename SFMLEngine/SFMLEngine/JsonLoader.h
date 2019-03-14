#pragma once
#include <nlohmann/json.hpp>
#include <fstream>
#include <iomanip>
#include "LogConsole.h"
#include <MetaStuff/Meta.h>

using json = nlohmann::json;
using namespace std;
namespace Engine
{
	class JsonLoader
	{
	protected:
		json j;
		std::string current_json;
	public:
		JsonLoader() { j.clear(); }
		~JsonLoader() = default;

		void LoadFromPath(std::string path);
		bool saveJson(std::string path, std::string json);

		template<class T>
		auto getValueByKey(std::string key)
		{
			j.clear();
			j = json::parse(current_json);
			return j[key].get<T>();
		}


	};
	template<class Obj>
	void saveObject(Obj* pObj)
	{
		assert(meta::isRegistered<Obj>(), "object is not registered");
		json j;

		meta::doForAllMembers<Obj>([&pObj, &j](const auto & member)
		{
			if (std::is_same<decltype(member.getCopy(*pObj)), float>())
			{
				float x = meta::getMemberValue<float>(*pObj, member.getName());
				j["Player"][member.getName()] = x;
			}
		});
		std::ofstream o("Data/save.json");
		o << std::setw(3) << j << std::endl;
		o.close();
	}

}
