#include "Game.h"

Engine::Game::Game(sf::RenderWindow & w)
{
	state = appState::Start;
	window = &w;
	world = new World();
	time.setDelta(500);
	m = new Engine::Menu("Data/GUI/MyUI/MainMenu.txt", w);
	state = appState::InMenu;
	m->makeMenu();
}

Engine::Game::~Game()
{
	delete world;
	delete m;
	delete window;
}

void Engine::Game::startGame()
{
	world->Init(*window);
}

void Engine::Game::update()
{
	while (window->isOpen())
	{
		time.Tick();
		sf::Event event;
		handleEvent(event);
		switch (state)
		{
		case Engine::Start:

			break;
		case Engine::Run:
			world->update(*window, time.getTime(), event); //call draw and handle event
			break;
		case Engine::Pause:

			break;
		case Engine::InMenu:
			m->update();
			state = appState::Run;
			break;
		default:
			break;
		}
	}
}

void Engine::Game::handleEvent(sf::Event & e)
{
	while (window->pollEvent(e))
	{
		if (e.type == sf::Event::Closed)
		{
			window->close();
			break;
		}
		world->handleEvent(e);
	}
}
