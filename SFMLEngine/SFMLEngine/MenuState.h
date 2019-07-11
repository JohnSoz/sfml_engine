#pragma once
#include "ApplicationState.h"
#include "MainMenu.h"

namespace Engine
{
	class MainState : public State
	{
	private:
		MainMenu* m;
		sf::RenderWindow* window;
		Clock deltaClock;
	public:
		MainState(sf::RenderWindow& win) :
			State(appState::UI), window(&win) {}
		~MainState() { Cleanup(); }
		void update(float time) override
		{
			m->update();
		}
		void handleEvent(sf::Event& e) override
		{
			m->handleEvent(e);
		}
		void draw() override
		{
			m->draw();
		}

		void Init() override { m = new MainMenu("Data/GUI/MyUI/MainMenu.txt", *window); m->makeMenu(); Initialized = true; }
		void Cleanup() override { delete m;  Initialized = false; }
	};

}

