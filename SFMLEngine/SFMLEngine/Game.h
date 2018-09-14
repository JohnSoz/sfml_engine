#pragma once
#include "World.h"
#include "GUI.h"
#include "timer.h"

namespace Engine
{
	enum appState
	{
		Start = 0,
		Run,
		Pause,
		InMenu
	};

	class Game
	{
	private:
		/**
		 * \brief 
		 */
		World*				world;
		Engine::Menu*		m;
		sf::RenderWindow*   window;
		sf::ContextSettings settings;
		std::string		    appname;
		sf::VideoMode	    videoMode;
		appState		    state;
		timer			    time;
	public:
		Game(sf::RenderWindow& w);
		~Game();
		void startGame();
		void update();
		void handleEvent(sf::Event& e);
	};
}
