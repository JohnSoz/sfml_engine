#include "LoadingState.h"
#include "LogConsole.h"
void Engine::LoadingState::Init(sf::RenderWindow& w)
{
	c.restart();
	window = &w; 
	screen = new LoadingScreen("Data/GUI/MyUI/MainMenu.txt", *window);
	screen->makeMenu();
	Initialized = true;
	completed = false;
	Console::AppLog::addLog(Console::Log("Engine::LoadingState::Init()", Console::logType::info));
}

void Engine::LoadingState::Cleanup()
{
	delete screen; 
	Initialized = false; 
	Console::AppLog::addLog(Console::Log("Engine::LoadingState::Cleanup()", Console::logType::info));
}
