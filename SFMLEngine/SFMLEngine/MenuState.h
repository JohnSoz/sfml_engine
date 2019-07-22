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
		MainState() :
			State(appState::UI)
		{
			nextState = appState::Play;
		}
		~MainState() { if (Initialized) Cleanup(); }
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

		void Init(sf::RenderWindow& w) override;
		void Cleanup() override;
	};

}

