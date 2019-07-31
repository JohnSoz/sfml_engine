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
		GameState() : State(appState::Play) {}
		~GameState() { if (Initialized) Cleanup(); }

		void update(float time) override;
		void updateImGui() override { world->updateImGui(); }
		void handleEvent(sf::Event& e) override { world->handleEvent(e); }
		void draw() override { world->draw(*window); }

		void Init(sf::RenderWindow& w) override;
		void Cleanup() override;
	};

}