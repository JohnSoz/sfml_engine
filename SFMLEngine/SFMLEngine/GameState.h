#pragma once
#include "ApplicationState.h"
#include "World.h"
#include <chrono>
#include "AudioPlayer.h"
using namespace std::chrono;
namespace Engine
{
	class GameState : public State
	{
	private:
		World* world;
		sf::RenderWindow* window;
		//MusicPlayer* musicPlayer;
	public:
		GameState(sf::RenderWindow& win) :
			State(appState::Play)
		{
			window = &win;
		}
		~GameState() { Cleanup(); }

		void updateImGui() override { world->updateImGui(); }

		void update(float time) override
		{
			world->update(*window, time);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2))
				EventManager::eventManager.emit<Events::Change_State_Event>(appState::UI);
		}

		void handleEvent(sf::Event& e) override
		{
			world->handleEvent(e);
		}

		void draw() override
		{
			world->draw(*window);
		}

		void Init() override { world = new World(); world->Init(*window); world->start(*window); Initialized = true; }
		void Cleanup() override { delete world; Initialized = false; }
	};

}