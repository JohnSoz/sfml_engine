#include "ApplicationState.h"
#include "LogConsole.h"
#include "EngineEvents.h"
#include <future>
#include <thread>
using namespace Engine;

Engine::StateStack::~StateStack()
{
	for (auto iter = states.begin(); iter != states.end(); ++iter)
		delete* iter;
	states.clear();
}

void Engine::StateStack::addState(State* state)
{
	Console::AppLog::addLog("A new state has been added to the state stack, new state id: " + std::to_string(state->getStateId()), Console::system);
	bool isFirst = false;
	if (states.empty())
		isFirst = true;
	states.push_back(state);
	if (isFirst)
	{
		currState = state;
		currState->Init(*window);
	}
}

bool Engine::StateStack::hasState(appState id)
{
	auto state = std::find_if(states.begin(), states.end(), [id](const State* state)
		{
			return id == state->getStateId();
		});
	return state != states.end();
}

void Engine::StateStack::changeState()
{
	if (currState->isCompleted())
	{
		auto state = std::find_if(states.begin(), states.end(), [Id = currState->getNextState()](const State* state)
		{
			return Id == state->getStateId();
		});
		if (state != states.end())
		{
			currState->Cleanup();
			currState = *state;
			if (!currState->Initialized)
				currState->Init(*window);
		}
		else
			Console::AppLog::addLog("changeState() occurred with an error", Console::logType::error);
	}
}

void Engine::StateStack::changeStateWithLoadingScreen(appState Id, actions action)
{
	auto state1 = std::find_if(states.begin(), states.end(), [Id](const State* state)
		{
			return Id == state->getStateId();
		});
	if (action != actions::none)
		(*state1)->setActions(action);
	auto task = [state1, this]()
	{
		sf::Context _;
		if (!(*state1)->Initialized)
			(*state1)->Init(*window);
	};		
	std::thread(task).detach();
	auto state2 = std::find_if(states.begin(), states.end(), [Id = appState::Loading](const State* state)
	{
		return Id == state->getStateId();
	});
	if (state2 != states.end())
	{
		currState->Cleanup();	
		currState = *state2;
		currState->setNextState(Id);
		if (!currState->Initialized)
			currState->Init(*window);
	}
}

void Engine::StateStack::changeStateTo(appState Id, actions action)
{
	auto state = std::find_if(states.begin(), states.end(), [Id](const State* state)
		{
			return Id == state->getStateId();
		});
	if (state != states.end())
	{
		currState->Cleanup();
		currState = *state;
		if (!currState->Initialized)
			currState->Init(*window);
	}
	else
		Console::AppLog::addLog("change of state occurred with an error", Console::logType::error);
}

State& Engine::StateStack::getState(appState state)
{
	return *(*std::find_if(states.begin(), states.end(), [state](const State* s) {return s->getStateId() == state; }));
}

void Engine::State::updateStatus()
{
	if (completed)
		EventManager::eventManager.emit<Events::Main_Menu_Event>(nextState);
}
