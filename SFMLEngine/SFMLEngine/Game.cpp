#include "Game.h"
#include "imgui.h"
#include "imgui-sfml.h"
#include "LogConsole.h"

Engine::Game::Game(sf::RenderWindow& w)
{
	state = appState::UI;
	window = &w;
	world = new World();
	time.setDelta(500);
	initMenu(w);

	EventManager::eventManager.subscribe<Events::EvSomeEvent>(*this);
	EventManager::eventManager.subscribe<Events::Event_UI>(*this);

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
	luabridge::getGlobalNamespace(L)
		.beginClass<Client>("Client")
		.addConstructor<void(*) ()>()
		.addFunction("sendPacket", &Client::sendMsg_l)
		.endClass();
	luabridge::push(L, &c);
	lua_setglobal(L, "L_Client");
	testWindow->addText("Text From C++");
	musicPlayer.Play();
}

void Engine::Game::update()
{
	while (window->isOpen())
	{
		auto local_last_state = state;

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
			camera.moveToPoint(world->getObjHendler().GetObjects<Actor>("Test").getPos(), *window);
			break;
		case Engine::UI:
			state = (appState)m->update();
			break;
		case Engine::Pause:
			break;
		}

		if (local_last_state != state)
		{
			isStateChange = !isStateChange;
			stateChanged();
		}

		draw();
	}
}

void Engine::Game::stateChanged()
{

	if (state == Engine::StartGame)
	{
		world->Init(*window);
		world->start();
		state = Engine::Play;
	}

	if (state == Engine::Pause)
		time.pause();

	if (state == Engine::Resume)
	{
		state = lastState;
		time.resume();
	}


	isStateChange = false;
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
	case Engine::Pause:
		break;
	}
	Console::AppLog::Draw("LogConsole", &LogConsole, L);
	ImGui::SFML::Render(*window);
	window->setView(camera.getView());
	window->display();
}

void Engine::Game::receive(const Events::Event_UI& ui_event)
{
	std::cout << ui_event.msg;
}

void Engine::Game::handleEvent(sf::Event& e)
{

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
	{
		time.pause();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
	{
		time.resume();
	}

	while (window->pollEvent(e))
	{
		if (e.type == sf::Event::Closed)
		{
			window->close();
			break;
		}

		if (e.type == sf::Event::LostFocus)
		{
			//VStaticContainer::windowIsActive = false;
			//lastState = state;
			//state = Pause;
		}

		if (e.type == sf::Event::GainedFocus)
		{
			//VStaticContainer::windowIsActive = true;
			//state = Resume;
		}

		switch (state)
		{
		case Engine::Play:
			world->handleEvent(e);
			break;
		case Engine::UI:
			m->handleEvent(e);
			break;
		case Engine::Pause:
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