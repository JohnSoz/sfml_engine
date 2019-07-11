#pragma once
#include "timer.h"
#include "ApplicationState.h"
#include <LuaBridge/LuaBridge.h>
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
	/*class A
	{
	public:
		std::string name;
		std::string text;
		std::string oneFrameText;
		ImVec2 size;
	public:
		A() = delete;
		A(std::string n)
		{
			size = { 400,400 };
			name = n;
			text.clear();
		}
		A(std::string n, float x, float y)
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
	};*/

	//class Game : public entityx::Receiver<Game>
	//{
	//private:
	//	World*      world;
	//	Clock Gtime;
	//	MainMenu*   m;
	//	MusicPlayer musicPlayer;
	//	A*          testWindow;
	//	//Client      c;
	//	float ftStep{ 1.f }, ftSlice{ 1.f };
	//	float lastFt{ 0.f };
	//	float currentSlice{ 0.f };
	//	lua_State*      L;
	//	RenderWindow*   window;
	//	ContextSettings settings;
	//	std::string     appname;
	//	std::string     path;

	//	Clock deltaClock;
	//	Clock pressClock;
	//	bool time;

	//	appState state;
	//	appState lastState;

	//	bool showConsole = false;
	//	bool LogConsole = false;
	//	bool isStateChange;

	//public:
	//	Game(sf::RenderWindow& w);
	//	~Game();
	//	void startGame();
	//	void update();
	//	void stateChanged();
	//	void draw();
	//	void initMenu(sf::RenderWindow& w);
	//	void receive(const Events::Main_Menu_Event& event);
	//	void handleEvent(sf::Event& e);
	//};

	class Game : public entityx::Receiver<Game>
	{
	private:
		appState newStateId;
		StateStack stack;
		sf::RenderWindow* window;
		Clock deltaClock;
		Clock pressClock;
		lua_State* L;

		float ftStep{ 1.f }, ftSlice{ 1.f };
		float lastFt{ 1.f };
		float currentSlice{ 0.f };
		bool LogConsole = false;
		bool needToChangeState = false;
		void handleEvent(sf::Event& e);
		void draw();
		void changeState()
		{
			if (newStateId == appState::Exits)
				window->close();
			stack.changeState(newStateId);
			needToChangeState = false;
		}
	public:
		Game(sf::RenderWindow& w);
		void start();
		StateStack& getStack() { return stack; }
		void receive(const Events::Change_State_Event& event);
	};
}
