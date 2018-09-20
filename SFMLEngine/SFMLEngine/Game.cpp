
#include "Game.h"
#include "imgui.h"
#include "imgui-sfml.h"

Engine::Game::Game(sf::RenderWindow & w)
{
	state = appState::UI;
	window = &w;
	world = new World();
	time.setDelta(500);
	m = new Engine::Menu("Data/GUI/MyUI/MainMenu.txt", w);
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
		ImGui::SFML::Update(*window, deltaClock.restart());
		std::cout << time;
		switch (state)
		{
		case Engine::Play:
			world->update(*window, time.getTime(), event);
			break;
		case Engine::UI:
			m->update();
			state = appState::Play;
			break;
		default:
			break;
		}
		draw();
	}
}

void Engine::Game::draw()
{
	window->clear(sf::Color::White);
	world->draw(*window);
	ImGui::SFML::Render(*window);
	window->display();
}

void Engine::Game::handleEvent(sf::Event & e)
{
	ImGui::SFML::ProcessEvent(e);
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
