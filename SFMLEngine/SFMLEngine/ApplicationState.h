#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
namespace Engine
{
	enum appState
	{
		Play = 1 << 0,
		MainWindow = 1 << 1,
		Exit = 1 << 2,
		StartGame = 1 << 3,
		Loading = 1 << 4
	};

	enum actions
	{
		none = 0,
		loading = 1,
		resume
	};

	class State
	{
	protected:
		appState stateId;
		appState nextState;
		actions  additionalAction;
		bool completed;
		void updateStatus();
	public:
		bool Initialized;
		State() = delete;
		State(appState id) : stateId(id) { Initialized = false; completed = false; }
		State(const State&) = delete;
		State(State&&) = default;
		virtual ~State() = default;

		appState getStateId()   const { return stateId; }
		appState getNextState() const { return nextState; }
		bool     isCompleted()  const { return completed; }

		void setActions(actions action) { additionalAction = action; }
		void setNextState(appState next) { nextState = next; }

		virtual void update(float time) = 0;
		virtual void fixedUpdate() {}
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
		void changeStateWithLoadingScreen(appState Id, actions action);
		void changeStateTo(appState Id, actions action);

		State& getCurrState() { return *currState; }
		State& getState(appState state);
	};
}