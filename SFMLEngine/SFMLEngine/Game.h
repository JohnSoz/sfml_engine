#pragma once
#include "World.h"
#include "GUI.h"
#include "timer.h"
#include "AudioPlayer.h"
#include <LuaBridge/LuaBridge.h>
extern "C" {
#include <Lua/lua.h>
#include <Lua/lauxlib.h>
#include <Lua/lualib.h>
}
namespace Engine
{
	enum appState
	{
		Play = 0,
		UI
	};
	class A
	{
	public:
		std::string name;
	    std::string text;
		std::string oneFrameText;
	public:
		A() = delete;
		A(std::string n)
		{
			name = n;
			text = "";
		}
		void addText(std::string t)
		{
			text += t;
		}
		void addText_l(std::string t)
		{
			oneFrameText += t;
		}
		void addWindow(bool isOpen)
		{
			if (isOpen)
			{
				ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);
				if (ImGui::Begin(name.c_str(), &isOpen))
				{
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
		Engine::Menu*		m;
		sf::RenderWindow*   window;
		AudioPlayer*        musicPlayer;
		sf::ContextSettings settings;
		std::string		    appname;
		sf::Clock           deltaClock;
		sf::View            camera;
		appState		    state;
		timer			    time;
		bool showConsole = false;
		sf::Clock pressClock;
	public:
		Game(sf::RenderWindow& w);
		~Game();
		void startGame();
		void update();
		void draw();
		void handleEvent(sf::Event& e);
	};
}
