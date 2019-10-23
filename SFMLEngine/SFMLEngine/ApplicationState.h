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

	/*
	 class State
	 {
		int stateId
		int nextState
		int  additionalAction
		...
	 public:
	  State(int state_ID)

	  template<class stateT>
	  stateT getStateId()

	  template<class stateT>
	  stateT getNextState()

	  void setActions(int action)
	  void setNextState(int next)
	  ...
	 };
	*/

	class State
	{
	protected:
		int stateId;
		int nextState;
		int  additionalAction;
		bool completed;
		void updateStatus();
	public:
		bool Initialized;
		State() = delete;
		State(int id) : stateId(id) { Initialized = false; completed = false; }
		State(const State&) = delete;
		virtual ~State() = default;

		int getStateId()   const { return stateId; }
		int getNextState() const { return nextState; }
		bool     isCompleted()  const { return completed; }

		void setActions(int action) { additionalAction = action; }
		void setNextState(int next) { nextState = next; }

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
		StateStack(sf::RenderWindow& w) : window(&w), currState(nullptr) { needToChangeState = false; }
		~StateStack();

		void addState(State* state);
		bool hasState(int id);
		void changeState();
		void asyncChangeState(int Id, int action);
		void changeStateTo(int Id, int action);

		State& getCurrState() { return *currState; }
		State& getState(int state);
	};
}