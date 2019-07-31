#include "MenuState.h"

void Engine::MainState::Init(sf::RenderWindow& w)
{
	mus->Play();
	window = &w;
	m = new MainMenu("Data/GUI/MyUI/MainMenu.txt", *window);
	m->makeMenu();
	Initialized = true;
	Console::AppLog::addLog(Console::Log("Engine::MainState::Init()", Console::logType::info));
}

void Engine::MainState::Cleanup()
{
	delete m;
	Initialized = false;
	mus->PauseMus();
	Console::AppLog::addLog(Console::Log("Engine::MainState::Cleanup()", Console::logType::info));
}
