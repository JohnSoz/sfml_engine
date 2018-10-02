#include <iostream>
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "DebuggingSystem.h"
#include <cmath>
#define _CRTDBG_MAP_ALLOC 
#include <stdlib.h>
#include <crtdbg.h> 
#include "LogConsole.h"
#include <LuaBridge/LuaBridge.h>
extern "C" {
#include <Lua/lua.h>
#include <Lua/lauxlib.h>
#include <Lua/lualib.h>
}
using namespace std;
using namespace luabridge;

//class A
//{
//public:
//	std::string name = "default";
//public:
//	A() = default;
//	void getAdr()
//	{
//		std::string log = "";
//
//		const void * address = static_cast<const void*>(this);
//		std::stringstream ss;
//		ss << address;
//		log += "&A " + ss.str() + ";  ";
//		ss.clear();
//
//		address = static_cast<const void*>(&this->name);
//		ss << address;
//		log += "&A::name " + ss.str() + ";";
//		ss.clear();
//
//		Console::AppLog::addLog(log, Console::logType::info);
//	}
//	void printName()
//	{
//		std::string log = "";
//		log += "A::name " + name + ";";
//		Console::AppLog::addLog(log, Console::logType::info);
//	}
//};
//class Test
//{
//public:
//	A* a;
//public:
//	Test() {  }
//	void setA(A* as)
//	{
//		a = as;
//	}
//	void setNewName(std::string s)
//	{
//		a->name = s;
//	}
//	void getAdr()
//	{
//		std::string log = "";
//
//		const void * address = static_cast<const void*>(this);
//		std::stringstream ss;
//		ss << address;
//		log += "&Test " + ss.str() + ";  ";
//		ss.clear();
//
//		address = static_cast<const void*>(this->a);
//		ss << address;
//		log += "&Test::a " + ss.str() + ";  ";
//		ss.clear();
//
//		address = static_cast<const void*>(&this->a->name);
//		ss << address;
//		log += "&Test::a->name " + ss.str() + ";";
//
//		Console::AppLog::addLog(log, Console::logType::info);
//	}
//};

//#main
int main()
{
	//lua_State* L = luaL_newstate();
	//luaL_openlibs(L);
	//luabridge::getGlobalNamespace(L)
	//	.beginClass<A>("A")
	//	.addConstructor<void(*) ()>()
	//	.addFunction("getAdr", &A::getAdr)
	//	.addFunction("printName", &A::printName)
	//	.endClass();
	//luabridge::getGlobalNamespace(L)
	//	.beginClass<Test>("Test")
	//	.addConstructor<void(*)()>()
	//	.addFunction("getAdr", &Test::getAdr)
	//	.addFunction("setNewName", &Test::setNewName)
	//	.addData("pointerA", &Test::a)
	//	.addFunction("setA", &Test::setA)
	//	.endClass();
	//luaL_dofile(L, "script.lua");
	//lua_pcall(L, 0, 0, 0);
	
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 16;
	settings.majorVersion = 3;
	settings.minorVersion = 0;
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML Game V0.00001", sf::Style::Default, settings);
	window.setFramerateLimit(350);
#if _Debug_
	ImGui::SFML::Init(window, true);
#endif
	Engine::DebuggingSystem::setWindow(window);
	Engine::Game g(window);
	g.startGame();
	g.update();
	return 0;
}