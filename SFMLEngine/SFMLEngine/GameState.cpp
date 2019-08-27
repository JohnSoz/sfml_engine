#include "GameState.h"
#include "Player.h"
void Engine::GameState::Init(sf::RenderWindow& w)
{
	Initialized = true;
	isPause = false;
	window = &w;
	world = new World();
	world->Init(*window);
	if (additionalAction == actions::loading)
		world->pushEntity(new Engine::Player(*window, world->getLevel()));
	else
		world->pushEntity(new Player(sf::Vector2f(120, 120), "Test", *window, world->getLevel(), "Data\\Animation\\TestAnim.xml"));
	pause = new PauseMenu("Data/GUI/MyUI/MainMenu.txt", *window);
	pause->makeMenu();
	pause->activateOrDisable();
	Console::AppLog::addLog(Console::Log("Engine::GameState::Init()", Console::logType::info));
}

void Engine::GameState::update(float time)
{
	if (!isPause)
		world->update(*window, time);
	else
		pause->update();
	//EventManager::eventManager.emit<Events::Change_State_Event>(appState::MainWindow);
}

void Engine::GameState::Cleanup()
{
	delete world;
	delete pause;
	window = nullptr;
	Initialized = false;
	Console::AppLog::addLog(Console::Log("Engine::GameState::Cleanup()", Console::logType::info));
}
