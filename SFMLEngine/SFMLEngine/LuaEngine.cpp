#include "LuaEngine.h"
sol::state Engine::Lua::LuaEngine::lua_state;
std::vector<std::string>  Engine::Lua::LuaEngine::script;