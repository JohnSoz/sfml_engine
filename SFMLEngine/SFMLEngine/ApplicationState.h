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

	class StateStack;

	class State
	{
	public:
		struct info
		{
			sf::RenderWindow* window;
			sf::Event* event;
			appState stateId;
			StateStack* state_stack;
		};
		info inf;

		State() = delete;
		State(State::info info) : inf(info) {}
		~State() = default;

		virtual void update(float time) = 0;
		virtual void handleEvent(sf::Event& e) = 0;
		virtual void draw() = 0;

		friend class StateStack;
	};

	class StateStack
	{
	private:
		std::vector<State*> states;
		State* currState;
	public:
		StateStack() = default;
		~StateStack() = default;

		void addState(State& state);
		void changeState(appState Id);

		const State& getState(appState state) const { return *currState; }

		void update(float t) { currState->update(t); }
		void handleEcent(sf::Event& ev) { currState->handleEvent(ev); }
		void draw() { currState->draw(); }
	};

}