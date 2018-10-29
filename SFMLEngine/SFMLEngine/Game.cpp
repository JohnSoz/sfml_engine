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
	m->makeMenu();
	testWindow = new A("TestLua");
}
Engine::Game::~Game()
{
	lua_close(L);
	delete world;
	delete m;
	delete testWindow;
}

void Engine::Game::startGame()
{
	L = luaL_newstate();
	luaL_openlibs(L);
	luabridge::getGlobalNamespace(L)
		.beginClass<A>("A")
		.addConstructor<void(*) (std::string)>()
		.addFunction("addWindow", &A::addWindow)
		.addFunction("addText", &A::addText_l)
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
		time.Tick();
		sf::Event event;
		handleEvent(event);
		ImGui::SFML::Update(*window, deltaClock.restart());
		luaL_dofile(L, "script.lua");

		bool stateChange = false;
		switch (state)
		{
		case Engine::Play:
			world->update(*window, time.getTime(), event);
			break;
		case Engine::UI:
			stateChange = m->update(event);
			break;
		}
		//camera.moveToPoint(world->getObjHendler().GetObjects<Actor>("Test").getPos());
		draw();
		if (stateChange) state = Engine::Play;
	}
}

void Engine::Game::draw()
{
	window->clear(sf::Color::Blue);

	switch (state)
	{
	case Engine::Play:
		world->draw(*window);
		break;
	case Engine::UI:
		m->draw();
		break;
	}

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
		ImGui::SFML::ProcessEvent(e);
		world->handleEvent(e);
	}
}
