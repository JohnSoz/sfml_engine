#include "Game.h"
#include "imgui.h"
#include "imgui-sfml.h"
#include "LogConsole.h"

Engine::Game::Game(sf::RenderWindow & w)
{
	state = appState::UI;
	window = &w;
	world = new World();
	time.setDelta(500);
	initMenu(w);
	m->makeMenu(path);
	testWindow = new A("TestLua");
	isStateChange = false;
}

void Engine::Game::initMenu(sf::RenderWindow& w)
{
	m = new Engine::MainMenu("Data/GUI/MyUI/MainMenu.txt", w);
}

Engine::Game::~Game()
{
	lua_close(L);
	delete world;
	delete testWindow;
}

void Engine::Game::startGame()
{
	Console::AppLog::addLog("Engine::Game::startGame()", Console::info);
	L = luaL_newstate();
	luaL_openlibs(L);
	luabridge::getGlobalNamespace(L)
		.beginClass<A>("A")
		.addConstructor<void(*) (std::string, float, float)>()
		.addFunction("addWindow", &A::addWindow)
		.addFunction("addText", &A::addText_l)
		.addFunction("getText", &A::getText_l)
		.endClass();
	luabridge::push(L, testWindow);
	lua_setglobal(L, "L_testWindow");
	testWindow->addText("Text From C++");
	world->Init(*window);
	musicPlayer.Play();
}

void Engine::Game::update()
{
	while (window->isOpen())
	{
		auto lastState = state;
		time.Tick();
		sf::Event event;
		handleEvent(event);
		ImGui::SFML::Update(*window, deltaClock.restart());

		luaL_dofile(L, "script.lua");

		switch (state)
		{
		case Exits:
			window->close();
			break;
		case Engine::Play:
			world->update(*window, time.getTime(), event);
			break;
		case Engine::UI:
			state = (appState)m->update();
			break;
		}
		if (lastState != state)
			isStateChange = true;
		else
			isStateChange = false;
		camera.moveToPoint(world->getObjHendler().GetObjects<Actor>("Test").getPos(), *window);
		draw();
		if (isStateChange)
			stateChanged();
	}
}

void Engine::Game::stateChanged()
{
	if (state == Play)
		world->start();
}

void Engine::Game::draw()
{
	window->clear();

	switch (state)
	{
	case Engine::Play:
		world->draw(*window);
		break;
	case Engine::UI:
		m->draw();
		break;
	}
	Console::AppLog::Draw("LogConsole", &LogConsole, L);
	ImGui::SFML::Render(*window);
	window->setView(camera.getView());
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

		switch (state)
		{
		case Engine::Play:
			world->handleEvent(e);
			break;
		case Engine::UI:
			m->handleEvent(e);
			break;
		}

		ImGui::SFML::ProcessEvent(e);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tilde))
	{
		if (pressClock.getElapsedTime().asMilliseconds() > 500)
		{
			LogConsole = !LogConsole;
			pressClock.restart();
		}
	}
}
