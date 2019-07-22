#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

namespace Engine
{
	enum appState
	{
		Play = 0, Pause, UI, Exits, StartGame, Resume, Loading
	};

	class State
	{
	protected:
		appState stateId;
		appState nextState;
		bool completed;
		void updateStatus();
	public:
		bool Initialized;
		State() = delete;
		State(appState id) : stateId(id) { Initialized = false; completed = false; }
		virtual ~State() = default;

		appState getStateId()   const { return stateId; }
		appState getNextState() const { return nextState; }
		bool     isCompleted()  const { return completed; }

		void setNextState(appState next) { nextState = next; }
		virtual void update(float time) = 0;
		virtual void updateImGui() {}
		virtual void handleEvent(sf::Event& e) = 0;
		virtual void draw() = 0;

		virtual void Init(sf::RenderWindow& w) = 0;
		virtual void Cleanup() = 0;
	};

	class StateStack
	{
	private:
		std::vector<State*> states;
		State* currState;
		sf::RenderWindow* window;
		bool needToChangeState;
	public:
		StateStack(sf::RenderWindow& w) : window(&w) { needToChangeState = false; }
		~StateStack();

		void addState(State* state);
		bool hasState(appState id);
		void changeState();
		void changeStateWithLoadingScreen(appState Id);
		void changeStateTo(appState Id);

		State& getCurrState() { return *currState; }
		State& getState(appState state);
	};
}