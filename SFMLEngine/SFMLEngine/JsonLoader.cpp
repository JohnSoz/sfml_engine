#include "JsonLoader.h"
#include <iomanip>

bool Engine::JsonLoader::saveJson(std::string path, std::string json)
{
	assert(!path.empty());
	std::ofstream o;
	try
	{
		o.exceptions(std::ios::badbit | std::ios::failbit);
		o.open(path);
	}
	catch (std::fstream::failure & err)
	{
		Console::AppLog::addLog("Exception open/read file: " + std::string(err.what()), Console::logType::error);
		return false;
	}
	j.clear();
	j = json::parse(json);
	o << std::setw(4) << j << std::endl;
	o.close();
}

void Engine::JsonLoader::LoadFromPath(std::string path)
{
	assert(!path.empty());
	std::ifstream i;
	try
	{
		i.exceptions(std::ios::badbit | std::ios::failbit);
		i.open(path);
	}
	catch (std::fstream::failure & err)
	{
		Console::AppLog::addLog("Exception opening/reading file: " + std::string(err.what()), Console::logType::error);
	}
	j.clear();
	i >> j;
	i.close();
	current_json =  j.dump();
}
