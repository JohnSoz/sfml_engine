#pragma once
#include "timer.h"
#include "ApplicationState.h"
#include <sol/sol.hpp>
#include <entityx/entityx.h>
extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include "EngineEvents.h"
#include "GameState.h"
#include "MenuState.h"
namespace Engine
{
	class TestLua
	{
	public:
		std::string name;
		std::string text;
		std::string oneFrameText;
		ImVec2 size;
	public:
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
		std::string getText_l()
		{
			return oneFrameText + text;
		}
		void addWindow(bool isOpen)
		{
			if (isOpen)
			{
				if (ImGui::Begin(name.c_str(), &isOpen, ImGuiWindowFlags_NoSavedSettings))
				{
					ImGui::SetWindowSize(size);
					ImGui::Text(text.c_str());
					ImGui::Separator();
					ImGui::Text(oneFrameText.c_str());
					ImGui::End();
				}
				oneFrameText.clear();
			}
		}
	};
	class Game : public entityx::Receiver<Game>
	{
	private:
		appState newStateId;
		StateStack stack;
		sf::RenderWindow* window;
		Clock deltaClock;
		Clock pressClock;
		sol::state lua_state;
		TestLua   test;

		float ftStep{ 1.f }, ftSlice{ 1.f };
		float lastFt{ 1.f };
		float currentSlice{ 0.f };
		bool LogConsole = false;
		bool needToChangeState = false;

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
