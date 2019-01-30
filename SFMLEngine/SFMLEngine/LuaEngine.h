#pragma once
#include <LuaBridge/LuaBridge.h>
extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
namespace Engine
{

	class LuaEngine
	{
	private:
		lua_State* lua_state;
	public:
		LuaEngine()
		{
			lua_state = luaL_newstate();
			luaL_openlibs(lua_state);
		}
		~LuaEngine() { lua_close(lua_state); }

		template<class T>
		auto addClass(std::string name)
		{
			return luabridge::getGlobalNamespace(lua_state).beginClass<T>(name.c_str());
		}

		auto getGlobalNamespace()
		{
			return luabridge::getGlobalNamespace(lua_state);
		}

		auto do_file(std::string path)
		{
			luaL_dofile(lua_state, path.c_str());
		}

		void update()
		{

		}
	};
}

