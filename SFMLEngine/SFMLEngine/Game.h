#pragma once
#include "World.h"
#include "GUI.h"


namespace Engine
{
	enum appState
	{
		Start = 0,
		Run,
		Pause
	};
	class Game
	{
	private:
		World *world;
		Engine::Menu *m;
		sf::RenderWindow* window;
		sf::ContextSettings settings;
		std::string appname;
		sf::VideoMode videoMode;
		appState state;
		struct time {
		private:
			static sf::Clock clock;
			static float t;
		public:
			static float getTime() { return t; }
			static void Tick()
			{
				t = clock.getElapsedTime().asMicroseconds();
				clock.restart();
				t = t / 500;
			}
		};
	public:
		Game(sf::RenderWindow& w)
		{
			window = &w;
			state = appState::Start;
			world = new World();
			m = new Engine::Menu("Data/GUI/Black.txt", w);
		}
		~Game()
		{
			delete world;
			delete m;
			delete window;
		}
		void startGame()
		{
			//	world->init();
			world->Init(*window);
			//world->update();
		}
		void update()
		{
			while (window->isOpen())
			{
				time::Tick();
				world->update(*window, time::getTime());
			}
		}
	};
}
