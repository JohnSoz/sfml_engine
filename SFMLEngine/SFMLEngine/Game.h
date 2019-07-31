#pragma once
#include "timer.h"
#include "EngineEvents.h"
#include "ApplicationState.h"
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <entityx/entityx.h>
#include "LuaEngine.h"

namespace Engine
{
	struct TestLua
	{
		std::string name;
		std::string text;
		std::string oneFrameText;
		ImVec2 size;
		bool isOpen;

		TestLua() = delete;
		TestLua(std::string n)
		{
			size = { 400,400 };
			name = n;
			text.clear();
		}
		TestLua(std::string n, float x, float y)
		{
			name = n;
			size.x = x;
			size.y = y;
			text.clear();
		}
		void addText(std::string t)
		{
			text += t;
		}
		void addText_l(std::string t)
		{
			oneFrameText += t;
		}
		static void registerClass()
		{
			Lua::LuaEngine::getLuaState().new_usertype<ImVec2>("vec2",
				sol::constructors<ImVec2(float, float)>(),
				"x", &ImVec2::x,
				"y", &ImVec2::y
				);
			Lua::LuaEngine::getLuaState().new_usertype<TestLua>("TestLua",
				sol::constructors<TestLua(std::string), TestLua(std::string, float, float)>(),
				"addText", &TestLua::addText,
				"addTextOne", &TestLua::addText_l,
				"clear", &TestLua::clear,
				"Begin", &TestLua::begin,
				"End", &ImGui::End,
				"text", &TestLua::Text,
				"size", &TestLua::size,
				"separator", &ImGui::Separator
				);
		}
		std::string getText_l()
		{
			return oneFrameText + text;
		}
		void clear()
		{
			oneFrameText.clear();
		}
		void Text(std::string s)
		{
			ImGui::Text(s.c_str());
		}
		void begin()
		{
			ImGui::Begin(name.c_str(), &isOpen, ImGuiWindowFlags_NoSavedSettings);
			ImGui::SetWindowSize(size);
		}
	};
	class Game : public entityx::Receiver<Game>
	{
	private:
		appState newStateId;
		actions stateAction;
		StateStack stack;
		sf::RenderWindow* window;
		sf::Clock deltaClock;
		sf::Clock pressClock;
		Lua::LuaEngine lua;

		float ftStep{ 1.f }, ftSlice{ 1.f };
		float lastFt{ 1.f };
		float currentSlice{ 0.f };
		bool LogConsole = false;
		bool needToChangeState = false;
		bool changeWithLoading = false;

		void handleEvent(sf::Event& e);
		void draw();
		void changeState();
	public:
		Game(sf::RenderWindow& w);
		~Game() { Console::AppLog::saveLog("Data\\log.txt"); }
		void start();
		void receive(const Events::Change_State_Event& event);

		StateStack& getStack() { return stack; }
	};
}
