#include "Game.h"
#include "imgui.h"
#include "imgui-sfml.h"
Engine::Game::Game(sf::RenderWindow & w)
{
	state = appState::UI;
	window = &w;
	world = new World();
	time.setDelta(500);
	m = new  Engine::Menu("Data/GUI/MyUI/MainMenu.txt", w);
	musicPlayer = new AudioPlayer();
	m->makeMenu();
	camera.reset(sf::FloatRect(0, 0, 1000, 563));
	window->setView(camera);
}
Engine::Game::~Game()
{
	delete world;
	delete m;
	delete musicPlayer;
}

void Engine::Game::startGame()
{
	world->Init(*window);
	musicPlayer->Play();
}

void Engine::Game::update()
{
	while (window->isOpen())
	{
		time.Tick();
		sf::Event event;
		handleEvent(event);
		ImGui::SFML::Update(*window, deltaClock.restart());
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
	while (window->pollEvent(e))
	{
		if (e.type == sf::Event::Closed)
		{
			window->close();
			break;
		}
		ImGui::SFML::ProcessEvent(e);
		world->handleEvent(e);
	}
}
