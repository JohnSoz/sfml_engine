#include "JsonLoader.h"

bool Engine::JsonLoader::saveJson(std::string json, std::string path)
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
		Console::AppLog::addLog("Exception opening/reading file: " + std::string(err.what()), Console::logType::error);
		return false;
	}
	j.clear();
	j.parse(json);
	o << j << std::endl;
}

std::string Engine::JsonLoader::LoadFromPath(std::string path)
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
		return "";
	}
	j.clear();
	i >> j;
	return j.dump();
}
