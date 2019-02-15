#pragma once
#include <SFML/Graphics.hpp>
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

	protected:
		info inf;
		const info& getInfo() const { return inf; }

	public:
		State() = delete;
		State(State::info info) : inf(info) {}
		~State() = default;

		virtual void update(float time) = 0;
		virtual void handleEvent(sf::Event& e) = 0;
		virtual void draw() = 0;
		void changeStateTo(appState id) { /**/ }

		friend class StateStack;
	};

	class TestState : public State
	{
	public:
		TestState(State::info inf) : State(inf) {}
		~TestState() = default;
		void update(float time) override {}
		void handleEvent(sf::Event& e) override {}
		void draw() override {}

	private:

	};


	class StateStack
	{
	private:
		std::vector<std::pair<appState, State*>> states;

	public:
		StateStack() = default;
		~StateStack()
		{
			for (auto iter = states.begin(); iter != states.end(); ++iter)
				delete iter->second;
			states.clear();
		}

		void addState(State & state) { states.push_back(std::pair(state.inf.stateId, &state)); }
		void replaceState(State & newState) { /*finds the status for state.info and replaces it*/ }
		void deleteState(appState state) { /*removes the state by its id*/ }
		const State& getState(appState state) const { }

		void update(float t) { for (const auto& state : states)  state.second->update(t); }
		void handleEcent(sf::Event & ev) { for (const auto& state : states) state.second->handleEvent(ev); }
		void draw() { for (auto& state : states) state.second->draw(); }

	};

}