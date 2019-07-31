#include "GameState.h"
#include <mutex>

void Engine::GameState::Init(sf::RenderWindow& w)
{
	w.setActive(true); //< for correct work in multithreading
	Initialized = true;
	window = &w;
	world = new World();
	world->Init(*window);
	if (additionalAction == actions::loading)
		world->load(w);
	else
		world->start(*window);
	Console::AppLog::addLog(Console::Log("Engine::GameState::Init()", Console::logType::info));
	w.setActive(false);
}

void Engine::GameState::update(float time)
{
	world->update(*window, time);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2))
		EventManager::eventManager.emit<Events::Change_State_Event>(appState::UI);
}

void Engine::GameState::Cleanup()
{
	delete world;
	window = nullptr;
	Initialized = false;
	Console::AppLog::addLog(Console::Log("Engine::GameState::Cleanup()", Console::logType::info));
}
