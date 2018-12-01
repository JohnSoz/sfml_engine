#pragma once
#include "World.h"
#include "GUI.h"
#include "timer.h"
#include "AudioPlayer.h"
#include "ApplicationState.h"
#include <LuaBridge/LuaBridge.h>

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

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

	class Game
	{
	private:
		/**
		 * \brief
		 */
		World*				world;
		lua_State*          L;
		A*                  testWindow;
		Engine::MainMenu*		m;
		sf::RenderWindow*   window;
		AudioPlayer        musicPlayer;
		sf::ContextSettings settings;
		std::string		    appname;
		sf::Clock           deltaClock;
		Camera              camera;
		appState		    state;
		timer			    time;
		bool showConsole = false;
		bool LogConsole = false;
		sf::Clock pressClock;
		std::string path;
		bool isStateChange;
	public:
		Game(sf::RenderWindow& w);
		~Game();
		void startGame();
		void update();
		void stateChanged();
		void draw();
		void initMenu(sf::RenderWindow& w);
		void handleEvent(sf::Event& e);
	};
}
