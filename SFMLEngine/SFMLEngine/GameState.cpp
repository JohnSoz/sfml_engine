#include "GameState.h"
#include <mutex>
void Engine::GameState::Init(sf::RenderWindow& w)
{
	Initialized = true;
	window = &w; 
	world = new World();
	world->Init(*window); 
	world->start(*window);
	Console::AppLog::addLog(Console::Log("Engine::GameState::Init()", Console::logType::info));
	window->setActive(false);
}

void Engine::GameState::Cleanup()
{
	delete world;
	window = nullptr; 
	Initialized = false;
	Console::AppLog::addLog(Console::Log("Engine::GameState::Cleanup()", Console::logType::info));
}
