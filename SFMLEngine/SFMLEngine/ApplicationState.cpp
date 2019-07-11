#include "ApplicationState.h"
#include "LogConsole.h"
using namespace Engine;

void Engine::StateStack::addState(State* state)
{
	bool isFirst = false;
	if (states.empty())
		isFirst = true;
	states.push_back(state);
	if (isFirst)
	{
		currState = state;
		currState->Init();
	}
}

void Engine::StateStack::changeState(appState Id)
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
			currState->Init();
	}
	else
		Console::AppLog::addLog("change of state occurred with an error", Console::logType::error);
}

State& Engine::StateStack::getState(appState state)
{
	return *(*std::find_if(states.begin(), states.end(), [state](const State* s) {return s->getStateId() == state; }));
}
