#pragma once
#include "World.h"
#include "GUI.h"
#include "timer.h"
#include "AudioPlayer.h"
namespace Engine
{
	enum appState
	{
		Play = 0,
		UI
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
