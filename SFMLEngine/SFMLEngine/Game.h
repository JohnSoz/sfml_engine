#pragma once
#include "World.h"
#include "MainMenu.h"
#include "timer.h"
#include "AudioPlayer.h"
#include "ApplicationState.h"
#include <LuaBridge/LuaBridge.h>
#include "Client.h"
#include "LuaEngine.h"
#include <entityx/entityx.h>
extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include "EngineEvents.h"
#include "Camera.h"
namespace Engine
{
	class A
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
	};

	class Game : public entityx::Receiver<Game>
	{
	private:
		World*				world;
		Engine::MainMenu*	m;
		MusicPlayer         musicPlayer;
		A*                  testWindow;
		Engine::Client c;

		lua_State*          L;
		LuaEngine           lua_eng;
		RenderWindow*   window;
		sf::ContextSettings settings;
		std::string		    appname;
		std::string         path;

		sf::Clock           deltaClock;
		sf::Clock           pressClock;
		timer			    time;

		Camera              camera;
		appState		    state;
		appState            lastState;

		bool                showConsole = false;
		bool                LogConsole = false;
		bool                isStateChange;

	public:
		Game(sf::RenderWindow& w);
		~Game();
		void startGame();
		void update();
		void stateChanged();
		void draw();
		void initMenu(sf::RenderWindow& w);
		void receive(const Events::Main_Menu_Event& event);
		void handleEvent(sf::Event& e);
	};
}
