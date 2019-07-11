#include "Game.h"
#include "imgui.h"
#include "imgui-SFML.h"
#include "LogConsole.h"
#include "staticVariable.h"

void Engine::Game::handleEvent(sf::Event& event)
{
	while (window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window->close();
			break;
		}
		stack.getCurrState().handleEvent(event);
		ImGui::SFML::ProcessEvent(event);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tilde) && pressClock.getElapsedTime().asMilliseconds() > 500)
	{
		LogConsole = !LogConsole;
		pressClock.restart();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1) && pressClock.getElapsedTime().asMilliseconds() > 500)
	{
		VStaticContainer::ShowGuiEditor = !VStaticContainer::ShowGuiEditor;
		pressClock.restart();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
		ftStep = 0.f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
		ftStep = 1.f;
}

void Engine::Game::draw()
{
	window->clear();
	stack.getCurrState().draw();
	Console::AppLog::Draw("LogConsole", &LogConsole, L);
	ImGui::SFML::Render(*window);
	window->display();
}

Engine::Game::Game(sf::RenderWindow& w) :
	window(&w)
{
	EventManager::eventManager.subscribe<Events::Change_State_Event>(*this);
	L = luaL_newstate();
	stack.addState(new MainState(*window));
	stack.addState(new GameState(*window));
}

void Engine::Game::start()
{
	while (window->isOpen())
	{
		auto timePoint1(chrono::high_resolution_clock::now());
		sf::Event event;
		handleEvent(event);
		currentSlice += lastFt;
		ImGui::SFML::Update(*window, deltaClock.restart());
		stack.getCurrState().updateImGui();
		for (; currentSlice >= ftSlice; currentSlice -= ftSlice)
			stack.getCurrState().update(ftStep);
		draw();
		auto timePoint2(chrono::high_resolution_clock::now());
		auto elapsedTime(timePoint2 - timePoint1);
		float ft{
			chrono::duration_cast<chrono::duration<float, milli>>(elapsedTime).count() };
		lastFt = ft;
		if (needToChangeState)
			changeState();
	}
}

void Engine::Game::receive(const Events::Change_State_Event& event)
{
	newStateId = (appState)event.id;
	needToChangeState = true;
}










//Engine::Game::Game(sf::RenderWindow& w)
//{
//	state = appState::UI;
//	window = &w;
//	world = new World();
//	//time.setDelta(600);
//	testWindow = new A("TestLua");
//	isStateChange = false;
//	EventManager::eventManager.subscribe<Events::Main_Menu_Event>(*this);
//	time = true;
//}
//
//void Engine::Game::initMenu(sf::RenderWindow& w)
//{
//	m = new Engine::MainMenu("Data/GUI/MyUI/MainMenu.txt", w);
//	m->makeMenu();
//}
//
//void Engine::Game::receive(const Events::Main_Menu_Event& event)
//{
//	std::string log = "receive Main_Menu_Event, state = " + std::to_string(event.state);
//	Console::AppLog::addLog(log, Console::info);
//	state = static_cast<appState>(event.state);
//}
//
//Engine::Game::~Game()
//{
//	lua_close(L);
//	delete world;
//	delete m;
//	delete testWindow;
//}
//
//void Engine::Game::startGame()
//{
//	Console::AppLog::addLog("Engine::Game::startGame()", Console::info);
//	initMenu(*window);
//	L = luaL_newstate();
//	luaL_openlibs(L);
//	//luabridge::getGlobalNamespace(L)
//	//	.beginClass<Camera>("Camera")
//	//	.addConstructor<void(*) ()>()
//	//	.addConstructor<void(*) (sf::Vector2f, sf::IntRect)>()
//	//	.addFunction("setPos", &Camera::setPosition)
//	//	.addFunction("test", &Camera::test)
//	//	.addFunction("setView", &Camera::setViewPort)
//	//	.endClass();
//	//luabridge::push(L, &camera);
//	//lua_setglobal(L, "cam");
//
//	luabridge::getGlobalNamespace(L)
//		.beginClass<A>("A")
//		.addConstructor<void(*) (std::string, float, float)>()
//		.addFunction("addWindow", &A::addWindow)
//		.addFunction("addText", &A::addText_l)
//		.addFunction("getText", &A::getText_l)
//		.endClass();
//	luabridge::push(L, testWindow);
//	lua_setglobal(L, "L_testWindow");
//
//	/*luabridge::getGlobalNamespace(L)
//		.beginClass<Client>("Client")
//		.addConstructor<void(*) ()>()
//		.addFunction("sendPacket", &Client::sendMsg_l)
//		.endClass();
//	luabridge::push(L, &c);
//	lua_setglobal(L, "L_Client");*/
//
//	testWindow->addText("Text From C++");
//	//musicPlayer.Play();
//}
//
//void Engine::Game::update()
//{
//	float fps = 0;
//	while (window->isOpen())
//	{
//		auto timePoint1(chrono::high_resolution_clock::now());
//
//		const auto local_last_state = state;
//		//time.Tick();
//		sf::Event event;
//		handleEvent(event);
//		currentSlice += lastFt;
//		ImGui::SFML::Update(*window, deltaClock.restart());
//		if (state == Play)
//			world->updateImGui();
//		for (; currentSlice >= ftSlice; currentSlice -= ftSlice)
//		{
//			switch (state)
//			{
//			case Exits:
//				window->close();
//				break;
//			case Engine::Play:
//				world->update(*window, (float)time);
//				break;
//			case Engine::UI:
//				m->update();
//				break;
//			case Engine::Pause:
//				break;
//			case Engine::Loading:
//				world->Init(*window);
//				world->load(*window);
//				state = appState::Play;
//				break;
//			}
//		}
//		//	ImGui::Text(std::to_string(fps).c_str());
//		if (local_last_state != state)
//		{
//			isStateChange = !isStateChange;
//			stateChanged();
//		}
//		draw();
//		auto timePoint2(chrono::high_resolution_clock::now());
//		auto elapsedTime(timePoint2 - timePoint1);
//		float ft{
//			chrono::duration_cast<chrono::duration<float, milli>>(elapsedTime)
//				.count() };
//
//		lastFt = ft;
//
//		auto ftSeconds(ft / 1000.f);
//		fps = 1.f / ftSeconds;
//
//	}
//}
//
//void Engine::Game::stateChanged()
//{
//	if (state == Engine::StartGame)
//	{
//		world->Init(*window);
//		world->start(*window);
//		state = Engine::Play;
//	}
//
//	if (state == Engine::Pause)
//		time = false;
//	//.pause();
//
//	if (state == Engine::Resume)
//	{
//		state = lastState;
//		time = true;//time.resume();
//	}
//
//
//	isStateChange = false;
//}
//
//void Engine::Game::draw()
//{
//	window->clear();
//	switch (state)
//	{
//	case Play:
//		world->draw(*window);
//		break;
//	case UI:
//		m->draw();
//		break;
//	case Pause:
//		break;
//	default:;
//	}
//	Console::AppLog::Draw("LogConsole", &LogConsole, L);
//	ImGui::SFML::Render(*window);
//	window->display();
//}
//
//void Engine::Game::handleEvent(sf::Event& e)
//{
//
//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
//	{
//		time = false;
//		//time.pause();
//	}
//
//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
//	{
//		time = true;
//		//time.resume();
//	}
//
//	while (window->pollEvent(e))
//	{
//		if (e.type == sf::Event::Closed)
//		{
//			window->close();
//			break;
//		}
//
//		if (e.type == sf::Event::LostFocus)
//		{
//			//VStaticContainer::windowIsActive = false;
//			//lastState = state;
//			//state = Pause;
//		}
//
//		if (e.type == sf::Event::GainedFocus)
//		{
//			//VStaticContainer::windowIsActive = true;
//			//state = Resume;
//		}
//
//		switch (state)
//		{
//		case Engine::Play:
//			world->handleEvent(e);
//			break;
//		case Engine::UI:
//			m->handleEvent(e);
//			break;
//		case Engine::Pause:
//			break;
//		case Exits: break;
//		case StartGame: break;
//		case Resume: break;
//		case Loading: break;
//		default:;
//		}
//		ImGui::SFML::ProcessEvent(e);
//	}
//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tilde) && pressClock.getElapsedTime().asMilliseconds() > 500)
//	{
//		LogConsole = !LogConsole;
//		pressClock.restart();
//	}
//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1) && pressClock.getElapsedTime().asMilliseconds() > 500)
//	{
//		VStaticContainer::ShowGuiEditor = !VStaticContainer::ShowGuiEditor;
//		pressClock.restart();
//	}
// }


