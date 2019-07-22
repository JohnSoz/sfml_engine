#pragma once
#include "ApplicationState.h"
#include "LoadingScreen.h"

namespace Engine
{
	class LoadingState : public State
	{
	private:
		sf::RenderWindow* window;
		sf::Time minLoadingTime;
		sf::Clock c;
		LoadingScreen* screen;
	public:
		LoadingState() :
			State(appState::Loading)
		{
			minLoadingTime = sf::seconds(15);
		}
		~LoadingState() { if (Initialized) Cleanup(); }

		void update(float time) override
		{
			screen->update();
			if (c.getElapsedTime() > minLoadingTime)
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