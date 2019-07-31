#pragma once
#include <sol/sol.hpp>
#include <vector>
namespace Engine
{
	namespace Lua
	{
		class LuaEngine
		{
		private:
			static sol::state lua_state;
			static std::vector<std::string> script;
		public:
			LuaEngine() { lua_state.open_libraries(sol::lib::base, sol::lib::math); }
			template<class... types>
			static void registerAllClass()
			{
				(types::registerClass(), ...);
			}
			static void addNewScript(std::string str)
			{
				script.emplace_back(str);
			}
			void runAllScript()
			{
				for (auto& path : script)
					lua_state.do_file(path);
			}
			void runScriptByName(std::string name)
			{
				auto s = std::find_if(script.begin(), script.end(), [name](const std::string& _name)
					{
						return name == _name;
					});
				if (s != script.end())
					lua_state.do_file(*s);
			}
			static sol::state& getLuaState() { return lua_state; }
		};
	}
}

