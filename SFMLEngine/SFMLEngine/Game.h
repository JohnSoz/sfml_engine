#pragma once
#include "timer.h"
#include "EngineEvents.h"
#include "ApplicationState.h"
#include <entityx/entityx.h>
#include "LogConsole.h"
namespace Engine
{
	class Game : public entityx::Receiver<Game>
	{
	private:
		appState newStateId;
		actions stateAction;
		StateStack stack;
		sf::RenderWindow* window;
		sf::Clock deltaClock;
		sf::Clock pressClock;

		float ftStep{ 1.f }, ftSlice{ 1.f };
		float lastFt{ 1.f };
		float currentSlice{ 0.f };
		bool LogConsole = false;
		bool needToChangeState = false;
		bool changeWithLoading = false;

		void handleEvent(sf::Event& e);
		void draw();
		void changeState();
	public:
		Game(sf::RenderWindow& w);
		~Game() { Console::AppLog::saveLog("Data\\log.txt"); }
		void start();
		void receive(const Events::Change_State_Event& event);

		StateStack& getStack() { return stack; }
	};
}
