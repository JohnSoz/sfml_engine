#pragma once
#include <nlohmann/json.hpp>
#include <fstream>
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
	public:
		JsonLoader() { j.clear(); }
		std::string LoadFromPath(std::string path);
		bool saveJson(std::string path, std::string json);
		~JsonLoader() = default;
	};
	class GameSave : public JsonLoader
	{
	public:
		bool savePos(std::string_view path, float x, float y)
		{
			json p;
			p.clear();
			p["Player"]["Position"]["X"] = x;
			p["Player"]["Position"]["Y"] = y;
			saveJson(path.data(), p.dump(4));
			return true;
		}
		template<class T>
		void save(T t)
		{
			meta::doForAllMembers<T>(
				[&](auto& member)
			{
				auto z = member.get(t);
			}
			);
		}
		std::pair<float, float> loadPos(std::string_view path)
		{
			json p = json::parse(LoadFromPath(path.data()));

			return std::pair(p["Player"]["Position"]["X"].get<float>(), p["Player"]["Position"]["Y"].get<float>());
		}
	};
}
