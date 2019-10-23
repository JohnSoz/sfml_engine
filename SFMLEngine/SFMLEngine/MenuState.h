#pragma once
#include "ApplicationState.h"
#include "MainMenu.h"
#include "AudioPlayer.h"

namespace Engine
{
	class MainState : public State
	{
	private:
		MainMenu* m;
		sf::RenderWindow* window;
		Clock deltaClock;
		MusicPlayer* mus;
	public:
		MainState() :
			State(appState::MainWindow)
		{
			nextState = appState::Play;
			mus = new MusicPlayer("Menu");
		}
		~MainState() { if (Initialized) Cleanup(); }
		void update(float time) override
		{
			mus->Update();
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

