#include "Game.h"
#include "imgui.h"
#include "imgui-SFML.h"
#include "LogConsole.h"
#include "staticVariable.h"
#include "GameState.h"
#include "MenuState.h"
#include "LoadingState.h"
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
	Console::AppLog::Draw("LogConsole", &LogConsole);
	ImGui::SFML::Render(*window);
	window->display();
}

void Engine::Game::changeState()
{
	if (newStateId == appState::Exit)
		window->close();
	else
	{
		if (changeWithLoading)
			stack.asyncChangeState(newStateId, stateAction);
		else
			stack.changeStateTo(newStateId, stateAction);
		needToChangeState = false;
	}
}

Engine::Game::Game(sf::RenderWindow& w) :
	window(&w), stack(w)
{
	EventManager::eventManager.subscribe<Events::Change_State_Event>(*this);
	stack.addState(new MainState());
	stack.addState(new GameState());
	stack.addState(new LoadingState());
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
		stack.getCurrState().fixedUpdate();
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
		stack.changeState();
	}
}

void Engine::Game::receive(const Events::Change_State_Event& event)
{
	newStateId = (appState)event.id;
	stateAction = (actions)event.action;
	changeWithLoading = event._flag;
	needToChangeState = true;
}
