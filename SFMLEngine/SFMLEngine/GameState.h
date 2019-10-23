#pragma once
#include "ApplicationState.h"
#include "World.h"
#include <chrono>
#include "AudioPlayer.h"
#include "PauseMenu.h"
using namespace std::chrono;

namespace Engine
{
	class GameState : public State
	{
	private:
		World* world;
		sf::RenderWindow* window;
		PauseMenu* pause;
		bool isPause;

	public:
		GameState() : State(appState::Play) { }
		~GameState() { if (Initialized) Cleanup(); }

		void update(float time) override;
		void fixedUpdate() override { if (!isPause) world->fixedUpdate(); }
		void handleEvent(sf::Event& e) override {
			if (e.type == e.KeyPressed && (e.key.code == sf::Keyboard::F2)) {
				pause->activateOrDisable();
				isPause = !isPause;
			}
			if (!isPause)
				world->handleEvent(e);
			else
				pause->handleEvent(e);
		}
		void draw() override { world->draw(*window); if (isPause) pause->draw(); }

		void Init(sf::RenderWindow& w) override;
		void Cleanup() override;
	};

}