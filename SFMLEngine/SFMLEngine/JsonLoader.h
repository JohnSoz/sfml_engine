#pragma once
#include <nlohmann/json.hpp>
#include <fstream>
#include "LogConsole.h"
using json = nlohmann::json;
using namespace std;
namespace Engine
{
	class JsonLoader
	{
	private:
		json j;
		bool saveJson(std::string json, std::string path);
	public:
		JsonLoader() { j.clear(); }
		std::string LoadFromPath(std::string path);
		~JsonLoader() = default;
	};
}
