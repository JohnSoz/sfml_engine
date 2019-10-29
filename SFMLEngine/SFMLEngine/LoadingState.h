#pragma once
#include "ApplicationState.h"
#include "LoadingScreen.h"
namespace Engine
{
	class LoadingState : public State
	{
	private:
		sf::RenderWindow* window;
		sf::Time LoadingTime;
		sf::Clock c;
		LoadingScreen* screen;
	public:
		LoadingState() :
			State(appState::Loading)
		{
			LoadingTime = sf::seconds(2);
		}
		~LoadingState() { if (Initialized) Cleanup(); }
		void update(float time) override
		{
			screen->update();
			if (c.getElapsedTime() > LoadingTime)
				completed = true;
			updateStatus();
		}
		void handleEvent(sf::Event& e) override
		{
			screen->handleEvent(e);
		}
		void draw() override
		{
			screen->draw();
		}

		void Init(sf::RenderWindow& w) override;
		void Cleanup() override;
	};
}