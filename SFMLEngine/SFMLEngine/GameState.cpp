#include "GameState.h"
#include <mutex>

void Engine::GameState::Init(sf::RenderWindow& w)
{
	w.setActive(true); //< for correct work in multithreading
	Initialized = true;
	window = &w;
	isPause = false;
	world = new World();
	pause = new PauseMenu("Data/GUI/MyUI/MainMenu.txt", *window);
	pause->makeMenu();
	pause->activateOrDisable();
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
	if (!isPause)
		world->update(*window, time);
	else
		pause->update();
	//EventManager::eventManager.emit<Events::Change_State_Event>(appState::UI);
}

void Engine::GameState::Cleanup()
{
	delete world;
	delete pause;
	window = nullptr;
	Initialized = false;
	Console::AppLog::addLog(Console::Log("Engine::GameState::Cleanup()", Console::logType::info));
}
