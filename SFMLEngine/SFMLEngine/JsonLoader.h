#pragma once
#include "json.hpp"
#include <fstream>
#include <iomanip>
#include "LogConsole.h"

using json = nlohmann::json;


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
		auto getValueByKey(const std::string& key)
		{
			j.clear();
			j = json::parse(current_json);
			return j[key].get<T>();
		}
	};

}