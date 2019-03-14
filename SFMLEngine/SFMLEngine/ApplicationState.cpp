#include "ApplicationState.h"
#include "LogConsole.h"
using namespace Engine;

void Engine::StateStack::addState(State& state)
{
	bool isFirst = false;
	if (states.empty())
		isFirst = true;
	states.push_back(&state);
	if (isFirst)
		currState = &state;
}

void Engine::StateStack::changeState(appState Id)
{
	auto state = std::find_if(states.begin(), states.end(), [Id](const State * state)
	{
		return Id == state->inf.stateId;
	});

	if (state != states.end())
		currState = *state;
	else
		Console::AppLog::addLog("change of state occurred with an error", Console::logType::error);
}
