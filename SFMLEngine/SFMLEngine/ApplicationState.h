#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <entityx/entityx.h>
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
		
	public:
		bool Initialized;
		State() = delete;
		State(appState id) : stateId(id) { Initialized = false; }
		virtual ~State() = default;

		appState getStateId() const { return stateId; }

		virtual void update(float time) = 0;
		virtual void updateImGui() {}
		virtual void handleEvent(sf::Event& e) = 0;
		virtual void draw() = 0;

		virtual void Init() = 0;
		virtual void Cleanup() = 0;
	};

	class StateStack : public entityx::Receiver<StateStack>
	{
	private:
		std::vector<State*> states;
		State* currState;
	public:
		StateStack() = default;
		~StateStack() = default;

		void addState(State* state);
		void changeState(appState Id);

		State& getCurrState()  { return *currState; }
		State& getState(appState state);
	};
}